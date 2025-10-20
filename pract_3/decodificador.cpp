#include "decodificador.h"
#include "codificador.h"

using namespace std;

string metodo1Decodificar(const string& binario, int n) {
    string resultado = "";
    size_t longitud = binario.length();

    if (longitud == 0) return "";

    // Separar en bloques
    string bloques[1000];
    int numBloques = 0;

    for (size_t i = 0; i < longitud; i += n) {
        string bloque = binario.substr(i, n);
        bloques[numBloques++] = bloque;
    }

    // Decodificar primer bloque
    for (size_t i = 0; i < bloques[0].length(); i++) {
        resultado += (bloques[0][i] == '0') ? '1' : '0';
    }

    // Decodificar bloques restantes
    for (int bloqueIdx = 1; bloqueIdx < numBloques; bloqueIdx++) {
        string bloqueAnterior = resultado.substr((bloqueIdx - 1) * n, n);
        int ceros, unos;
        contarBitsString(bloqueAnterior, ceros, unos);

        string bloqueActual = bloques[bloqueIdx];
        string bloqueDecodificado = "";

        if (ceros == unos) {
            for (size_t i = 0; i < bloqueActual.length(); i++) {
                bloqueDecodificado += (bloqueActual[i] == '0') ? '1' : '0';
            }
        } else if (ceros > unos) {
            for (size_t i = 0; i < bloqueActual.length(); i += 2) {
                if (i + 1 < bloqueActual.length()) {
                    bloqueDecodificado += (bloqueActual[i + 1] == '0') ? '1' : '0';
                    bloqueDecodificado += (bloqueActual[i] == '0') ? '1' : '0';
                } else {
                    bloqueDecodificado += (bloqueActual[i] == '0') ? '1' : '0';
                }
            }
        } else {
            for (size_t i = 0; i < bloqueActual.length(); i += 3) {
                for (int k = 2; k >= 0; k--) {
                    if (i + k < bloqueActual.length()) {
                        bloqueDecodificado += (bloqueActual[i + k] == '0') ? '1' : '0';
                    }
                }
            }
        }

        resultado += bloqueDecodificado;
    }

    return resultado;
}

string metodo2Decodificar(const string& binario, int n) {
    string resultado = "";
    size_t longitud = binario.length();

    for (size_t i = 0; i < longitud; i += n) {
        string bloque = binario.substr(i, n);

        if (!bloque.empty()) {
            // Rotación inversa
            string bloqueDecodificado = bloque.substr(1);
            bloqueDecodificado += bloque[0];
            resultado += bloqueDecodificado;
        }
    }

    return resultado;
}

void decodificarConString(int n, int metodo, const string& archivoEntrada, const string& archivoSalida) {
    try {
        FILE* archivo = fopen(archivoEntrada.c_str(), "rb");
        if (!archivo) {
            throw "No se pudo abrir el archivo de entrada";
        }

        fseek(archivo, 0, SEEK_END);
        long tam = ftell(archivo);
        fseek(archivo, 0, SEEK_SET);

        if (tam <= 0) {
            fclose(archivo);
            throw "El archivo de entrada está vacío";
        }

        char* buffer = new char[tam + 1];
        size_t leidos = fread(buffer, 1, tam, archivo);
        if (leidos != (size_t)tam) {
            delete[] buffer;
            fclose(archivo);
            throw "Error al leer el archivo de entrada";
        }
        buffer[tam] = '\0';
        fclose(archivo);

        string contenido(buffer, tam);
        delete[] buffer;

        string binario = stringToBinary(contenido);

        string binarioDecodificado;
        if (metodo == 1) {
            binarioDecodificado = metodo1Decodificar(binario, n);
        } else {
            binarioDecodificado = metodo2Decodificar(binario, n);
        }

        string textoDecodificado = binaryToString(binarioDecodificado);

        FILE* salida = fopen(archivoSalida.c_str(), "wb");
        if (!salida) {
            throw "No se pudo crear el archivo de salida";
        }

        fwrite(textoDecodificado.c_str(), 1, textoDecodificado.length(), salida);
        fclose(salida);

        cout << "Archivo decodificado exitosamente" << endl;

    } catch (const char* mensaje) {
        cerr << "Error: " << mensaje << endl;
    } catch (...) {
        cerr << "Error desconocido durante la decodificación" << endl;
    }
}
