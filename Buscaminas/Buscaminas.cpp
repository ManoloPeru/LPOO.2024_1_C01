#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const char MINA = 'M';
const char CASILLA_SIN_DESCUBRIR = 'O';
const char CASILLA_VACIA = ' ';
const int MAX = 30; //Determina el maximo de fila y columna que puede tener el tablero

void InicializaTablero(char tableroJuego[MAX][MAX], int iFilas, int iColumnas) {
    for (int i = 0; i < iFilas; ++i) {
        for (int j = 0; j < iColumnas; ++j) {
            tableroJuego[i][j] = CASILLA_SIN_DESCUBRIR;
        }
    }
}

void InicializaTableroNumeros(char tableroMinasNumeros[MAX][MAX], int iFilas, int iColumnas) {
    for (int i = 0; i < iFilas; ++i) {
        for (int j = 0; j < iColumnas; ++j) {
            tableroMinasNumeros[i][j] = CASILLA_VACIA;
        }
    }
}

void ColocaMinas(char tableroJuego[MAX][MAX], int iFilas, int iColumnas, int iNumMinas) {
    int iMinasColocadas = 0;
    while (iMinasColocadas < iNumMinas) {
        int fila = rand() % iFilas;
        int columna = rand() % iColumnas;
        if (tableroJuego[fila][columna] != MINA) {
            tableroJuego[fila][columna] = MINA;
            iMinasColocadas++;
        }
    }
}

bool EsValida(int fila, int columna, int iFilas, int iColumnas) {
    return (fila >= 0 && fila < iFilas && columna >= 0 && columna < iColumnas);
}

void ColocaNumeros(char tableroMinasNumeros[MAX][MAX], int iFilas, int iColumnas) {
    int movimientosPermitidos[8][2] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

    for (int i = 0; i < iFilas; ++i) {
        for (int j = 0; j < iColumnas; ++j) {
            if (tableroMinasNumeros[i][j] == MINA) {
                continue;
            }

            int iNumMinasVecinas = 0;
            for (int k = 0; k < 8; ++k) {
                int iNuevaFila = i + movimientosPermitidos[k][0];
                int iNuevaColumna = j + movimientosPermitidos[k][1];
                if (EsValida(iNuevaFila, iNuevaColumna, iFilas, iColumnas) && 
                    tableroMinasNumeros[iNuevaFila][iNuevaColumna] == MINA) {
                    iNumMinasVecinas++;
                }
            }
            if (iNumMinasVecinas > 0) {
                //Nos permite almacenar el numero de minas vecinas como un caracter: '1', '2', '3', etc.
                tableroMinasNumeros[i][j] = '0' + iNumMinasVecinas;
            }
        }
    }
}

void ImprimeTablero(char tableroJuego[MAX][MAX], int iFilas, int iColumnas) {
    for (int i = 0; i < iFilas; ++i) {
        for (int j = 0; j < iColumnas; ++j) {
            cout << tableroJuego[i][j] << " ";
        }
        cout << endl;
    }
}

bool VerificaGanador(char tableroJuego[MAX][MAX], int iFilas, int iColumnas, int iNumMinas) {
    int iCasillasSinMina = iFilas * iColumnas - iNumMinas;
    for (int i = 0; i < iFilas; ++i) {
        for (int j = 0; j < iColumnas; ++j) {
            if (tableroJuego[i][j] != MINA && tableroJuego[i][j] != CASILLA_SIN_DESCUBRIR) {
                iCasillasSinMina--;
            }
        }
    }
    return iCasillasSinMina == 0;
}

int main() {
    srand(time(0)); // Inicializar la semilla para generar números aleatorios

    cout << "Bienvenido al juego de Buscaminas" << endl;
    cout << "Los niveles son:" << endl;
    cout << "(1) Principiante" << endl;
    cout << "(2) Intermedio" << endl;
    cout << "(3) Experto" << endl;
    cout << "(4) Personalizado" << endl;

    int iOpcion;
    cout << "Ingrese opción: ";
    cin >> iOpcion;

    int iFilas, iColumnas, iNumMinas;
    if (iOpcion == 4) {
        cout << "Ingrese la cantidad de minas: ";
        cin >> iNumMinas;
        cout << "Ingrese las dimensiones del tablero (NxM): ";
        cin >> iFilas >> iColumnas;
    }
    else {
        // Definir iFilas, iColumnas y minas según la opción elegida
        if (iOpcion == 1) {
            iFilas = 8;
            iColumnas = 8;
            iNumMinas = 10;
        }
        else if (iOpcion == 2) {
            iFilas = 16;
            iColumnas = 16;
            iNumMinas = 40;
        }
        else if (iOpcion == 3) {
            iFilas = 16;
            iColumnas = 30;
            iNumMinas = 99;
        }
    }

    char tableroJuego[MAX][MAX];
    char tableroMinasNumeros[MAX][MAX];

    // Inicializar los tableros
    InicializaTablero(tableroJuego, iFilas, iColumnas);
    InicializaTableroNumeros(tableroMinasNumeros, iFilas, iColumnas);
    ColocaMinas(tableroMinasNumeros, iFilas, iColumnas, iNumMinas);
    ColocaNumeros(tableroMinasNumeros, iFilas, iColumnas);

    cout << "El tablero ha sido creado, puede empezar a jugar." << endl;
    
    cout << "El tablero de numeros es:" << endl;
    ImprimeTablero(tableroMinasNumeros, iFilas, iColumnas);

    cout << "El tablero actual es:" << endl;
    ImprimeTablero(tableroJuego, iFilas, iColumnas);
    
    bool gameover = false;
    while (!gameover) {
        int fila, columna;
        cout << "Ingrese la fila y columna de la casilla a voltear: ";
        cin >> fila >> columna;

        if (fila < 1 || fila > iFilas || columna < 1 || columna > iColumnas) {
            cout << "Posición fuera del rango. Intente de nuevo." << endl;
            continue;
        }

        // Convertir a índices de arreglo (restamos 1 para ajustar al índice base 0)
        fila--;
        columna--;

        if (tableroMinasNumeros[fila][columna] == MINA) {
            cout << "Boom! Perdiste, has encontrado una mina" << endl;
            cout << "Fin del juego." << endl;
            gameover = true;
        }
        else {
            tableroJuego[fila][columna] = tableroMinasNumeros[fila][columna]; // Voltear la casilla

            // Imprimir el tablero actual
            cout << "El tablero actual es:" << endl;
            ImprimeTablero(tableroJuego, iFilas, iColumnas);

            // Verificar si el jugador ganó
            gameover = VerificaGanador(tableroJuego, iFilas, iColumnas, iNumMinas);
            if (gameover) {
                cout << "¡Felicidades! Has ganado." << endl;
                break;
            }
        }
    }

    return 0;
}
