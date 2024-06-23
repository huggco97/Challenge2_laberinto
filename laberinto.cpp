
#include <iostream>
#include <ctime>
#include <stack>
#include <thread>
#include <chrono>

using namespace std;

// Estructura para representar una posición en el laberinto
struct Posicion {
    int x, y;
};

// Función para imprimir el laberinto
void imprimir_laberinto(int **matriz, int filas, int columnas, bool **visitado) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (i == 1 && j == 0) {
                cout << "E"; // Entrada
            } else if (i == filas - 2 && j == columnas - 1) {
                cout << "S"; // Salida
            } else if (matriz[i][j] == 1) {
                cout << "#";
            } else if (visitado[i][j]) {
                cout << "\033[32m*\033[0m"; // cambiar color al asterisco
            } else {
                cout << ".";
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

void generar_laberinto(int filas, int columnas, int densidad) {
    int **matriz;
    matriz = new int *[filas];
    int Fparedes = densidad * 8;
    densidad = filas * columnas * densidad / 4;
    for (int i = 0; i < filas; i++) {
        matriz[i] = new int[columnas];
    }

    // Inicializar matriz con bordes y espacios
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (i == 0 || j == 0 || i == filas - 1 || j == columnas - 1) {
                matriz[i][j] = 1;
            } else {
                matriz[i][j] = 0;
            }
        }
    }

    // Generar paredes aleatorias
    srand(time(NULL));
    for (int i = 0; i < densidad; i++) {
        int x = rand() % (columnas - 4) + 2;
        x = (x / 2) * 2;
        int y = rand() % (filas - 4) + 2;
        y = (y / 2) * 2;

        // Verificar que no se generen muros en la fila de la salida
        if (y == filas - 2) {
            continue;
        }

        matriz[y][x] = 1;
        for (int j = 0; j < Fparedes; j++) {
            int mx[4] = {x, x, x + 2, x - 2};
            int my[4] = {y + 2, y - 2, y, y};
            int r = rand() % 4;
            if (matriz[my[r]][mx[r]] == 0) {
                matriz[my[r]][mx[r]] = 1;
                matriz[my[r] + (y - my[r]) / 2][mx[r] + (x - mx[r]) / 2] = 1;
            }
        }
    }

    // Establecer entrada y salida
    matriz[1][0] = 0; // Entrada
    matriz[filas - 2][columnas - 2] = 0; // Salida

    // Resolver laberinto
    stack<Posicion> camino;
    stack<stack<Posicion>> opciones_pendientes;
    bool **visitado = new bool *[filas];
    for (int i = 0; i < filas; i++) {
        visitado[i] = new bool[columnas];
        for (int j = 0; j < columnas; j++) {
            visitado[i][j] = false;
        }
    }

    Posicion inicio = {0, 1};
    Posicion fin = {columnas - 2, filas - 2};
    camino.push(inicio);
    visitado[inicio.y][inicio.x] = true;
    bool encontrado = false;

    int dirX[4] = {0, 1, 0, -1};
    int dirY[4] = {1, 0, -1, 0};

    while (!camino.empty() && !encontrado) {
        Posicion actual = camino.top();
        imprimir_laberinto(matriz, filas, columnas, visitado);
        this_thread::sleep_for(chrono::milliseconds(200));

        if (actual.x == fin.x && actual.y == fin.y) {
            encontrado = true;
            break;
        }

        bool hay_opciones = false;
        stack<Posicion> opciones;
        for (int i = 0; i < 4; i++) {
            int nuevoX = actual.x + dirX[i];
            int nuevoY = actual.y + dirY[i];
            if (nuevoX >= 0 && nuevoX < columnas && nuevoY >= 0 && nuevoY < filas &&
                matriz[nuevoY][nuevoX] == 0 && !visitado[nuevoY][nuevoX]) {
                opciones.push({nuevoX, nuevoY});
                hay_opciones = true;
            }
        }

        if (hay_opciones) {
            camino.push(actual);
            opciones_pendientes.push(opciones);
            Posicion siguiente = opciones.top();
            opciones.pop();
            visitado[siguiente.y][siguiente.x] = true;
            camino.push(siguiente);
            if (!opciones.empty()) {
                opciones_pendientes.push(opciones);
            }
        } else {
            visitado[actual.y][actual.x] = false;
            camino.pop();
            if (!camino.empty() && !opciones_pendientes.empty()) {
                camino.pop();
                opciones = opciones_pendientes.top();
                opciones_pendientes.pop();
                Posicion siguiente = opciones.top();
                opciones.pop();
                visitado[siguiente.y][siguiente.x] = true;
                camino.push(siguiente);
                if (!opciones.empty()) {
                    opciones_pendientes.push(opciones);
                }
            }
        }
    }

    // Pintar laberinto final
    imprimir_laberinto(matriz, filas, columnas, visitado);
    
    if (encontrado) {
        cout << "Escapaste del laberinto\n";
    } 

    // Liberar memoria
    for (int i = 0; i < filas; i++) {
        delete[] matriz[i];
        delete[] visitado[i];
    }
    delete[] matriz;
    delete[] visitado;
}

int main() {
    int filas, columnas;
    float densidad = 1;
    //validacion para el numero de filas
    do{
        cout << "Ingrese el numero de filas mayor o igual 11: ";
        cin >> filas;
        if(filas < 11){
            cout << "El numero de filas debe ser mayor o igual a 11. Intente de nuevo.\n";
        }
    }while (filas < 11);
    //validacion para el numero de columnas
    do{
        cout << "Ingrese el numero de columnas mayor o igual a 11: ";
        cin >> columnas;
        if (columnas < 11){
            cout << "El numero de columnas debe ser mayor o igual a 11. Intente de nuevo.\n";
        }
    }while (columnas < 11);
    generar_laberinto(filas, columnas, densidad);
    cout << "FIN \n";
    return 0;
}
