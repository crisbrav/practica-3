#include "codificador.h"
#include "decodificador.h"
#include "sistema_bancario.h"

using namespace std;

int main(int argc, char* argv[]) {
    try {
        if (argc > 1) {
            if (strcmp(argv[1], "codificar") == 0 && argc == 6) {
                int semilla, metodo;

                // Convertir parámetros
                semilla = 0;
                for (int i = 0; argv[2][i] != '\0'; i++) {
                    if (argv[2][i] >= '0' && argv[2][i] <= '9') {
                        semilla = semilla * 10 + (argv[2][i] - '0');
                    }
                }

                metodo = 0;
                for (int i = 0; argv[3][i] != '\0'; i++) {
                    if (argv[3][i] >= '0' && argv[3][i] <= '9') {
                        metodo = metodo * 10 + (argv[3][i] - '0');
                    }
                }

                if (semilla <= 0 || (metodo != 1 && metodo != 2)) {
                    throw "Parámetros inválidos";
                }

                codificarConString(semilla, metodo, argv[4], argv[5]);

            } else if (strcmp(argv[1], "decodificar") == 0 && argc == 6) {
                int semilla, metodo;

                semilla = 0;
                for (int i = 0; argv[2][i] != '\0'; i++) {
                    if (argv[2][i] >= '0' && argv[2][i] <= '9') {
                        semilla = semilla * 10 + (argv[2][i] - '0');
                    }
                }

                metodo = 0;
                for (int i = 0; argv[3][i] != '\0'; i++) {
                    if (argv[3][i] >= '0' && argv[3][i] <= '9') {
                        metodo = metodo * 10 + (argv[3][i] - '0');
                    }
                }

                if (semilla <= 0 || (metodo != 1 && metodo != 2)) {
                    throw "Parámetros inválidos";
                }

                decodificarConString(semilla, metodo, argv[4], argv[5]);

            } else {
                cout << "Uso:" << endl;
                cout << "  Codificar: programa codificar <semilla> <metodo> <entrada> <salida>" << endl;
                cout << "  Decodificar: programa decodificar <semilla> <metodo> <entrada> <salida>" << endl;
                cout << "  Método: 1 o 2" << endl;
            }
        } else {
            sistemaBancario();
        }
    } catch (const char* mensaje) {
        cerr << "Error: " << mensaje << endl;
    } catch (...) {
        cerr << "Error desconocido en el programa principal" << endl;
    }

    return 0;
}
