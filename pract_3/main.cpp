#include "codificador.h"
#include "decodificador.h"
#include "sistema_bancario.h"

using namespace std;

// Función mejorada para convertir argumentos
unsigned long stringAUnsignedLong(const char* str) {
    try {
        if (str == NULL || str[0] == '\0') {
            throw "String vacio o nulo";
        }

        unsigned long resultado = 0;
        bool tieneDigitos = false;

        for (int i = 0; str[i] != '\0'; i++) {
            if (str[i] >= '0' && str[i] <= '9') {
                resultado = resultado * 10 + (str[i] - '0');
                tieneDigitos = true;

                // Verificar overflow
                if (resultado > 1000000) {
                    throw "Numero demasiado grande";
                }
            } else if (str[i] != ' ' && str[i] != '\t') {
                throw "Caracter no numerico encontrado";
            }
        }

        if (!tieneDigitos) {
            throw "No se encontraron digitos numericos";
        }

        return resultado;
    } catch (const char* mensaje) {
        throw string("Error conversión a numero: ") + mensaje;
    }
}

int main(int argc, char* argv[]) {
    try {
        cout << " SISTEMA DE CODIFICACION/DECODIFICACION " << endl;

        if (argc > 1) {
            if (strcmp(argv[1], "codificar") == 0) {
                if (argc != 6) {
                    throw "Uso: programa codificar <semilla> <metodo> <entrada> <salida>";
                }

                // CONVERSIÓN CON MANEJO DE EXCEPCIONES
                unsigned long semilla = stringAUnsignedLong(argv[2]);
                unsigned long metodoUL = stringAUnsignedLong(argv[3]);

                if (metodoUL > 2) {
                    throw "El metodo debe ser 1 o 2";
                }
                int metodo = (int)metodoUL;

                cout << "Iniciando codificacion..." << endl;
                cout << "Parametros: semilla=" << semilla << ", metodo=" << metodo << endl;

                codificarConString(semilla, metodo, argv[4], argv[5]);

            } else if (strcmp(argv[1], "decodificar") == 0) {
                if (argc != 6) {
                    throw "Uso: programa decodificar <semilla> <metodo> <entrada> <salida>";
                }

                unsigned long semilla = stringAUnsignedLong(argv[2]);
                unsigned long metodoUL = stringAUnsignedLong(argv[3]);

                if (metodoUL > 2) {
                    throw "El metodo debe ser 1 o 2";
                }
                int metodo = (int)metodoUL;

                cout << "Iniciando decodificacion..." << endl;
                cout << "Parametros: semilla=" << semilla << ", metodo=" << metodo << endl;

                decodificarConString(semilla, metodo, argv[4], argv[5]);

            } else {
                throw string("Comando desconocido: ") + argv[1];
            }
        } else {
            // Modo interactivo
            sistemaBancario();
        }

        cout << "PROCESO COMPLETADO" << endl;

    } catch (const char* mensaje) {
        cerr << " Error en programa principal: " << mensaje << endl;
        cerr << "Uso correcto:" << endl;
        cerr << "  Codificar: programa codificar <semilla> <metodo> <entrada> <salida>" << endl;
        cerr << "  Decodificar: programa decodificar <semilla> <metodo> <entrada> <salida>" << endl;
        cerr << "  Sistema bancario: programa (sin argumentos)" << endl;
        return 1;
    } catch (const string& mensaje) {
        cerr << " Error en programa principal: " << mensaje << endl;
        return 1;
    } catch (...) {
        cerr << " Error desconocido en programa principal" << endl;
        return 1;
    }

    return 0;
}
