#include "codificador.h"

using namespace std;

string stringToBinary(const string& texto) {
    string binario = "";
    for (size_t i = 0; i < texto.length(); i++) {
        char c = texto[i];
        for (int j = 7; j >= 0; j--) {
            binario += ((c >> j) & 1) ? '1' : '0';
        }
    }
    return binario;
}

string binaryToString(const string& binario) {
    string resultado = "";
    for (size_t i = 0; i < binario.length(); i += 8) {
        if (i + 8 <= binario.length()) {
            string byteStr = binario.substr(i, 8);
            char c = 0;
            for (int j = 0; j < 8; j++) {
                c = (c << 1) | (byteStr[j] - '0');
            }
            resultado += c;
        }
    }
    return resultado;
}

void contarBitsString(const string& bloque, int& ceros, int& unos) {
    ceros = 0;
    unos = 0;
    for (size_t i = 0; i < bloque.length(); i++) {
        if (bloque[i] == '0') ceros++;
        else if (bloque[i] == '1') unos++;
    }
}

string metodo1String(const string& binario, int n) {
    string resultado = "";
    size_t longitud = binario.length();

    // Procesar primer bloque
    for (int i = 0; i < n && i < (int)longitud; i++) {
        resultado += (binario[i] == '0') ? '1' : '0';
    }

    // Procesar bloques restantes
    for (size_t inicioActual = n; inicioActual < longitud; inicioActual += n) {
        size_t inicioAnterior = inicioActual - n;

        string bloqueAnterior = binario.substr(inicioAnterior, n);
        int ceros, unos;
        contarBitsString(bloqueAnterior, ceros, unos);

        string bloqueActual = binario.substr(inicioActual, n);

        if (ceros == unos) {
            for (size_t i = 0; i < bloqueActual.length(); i++) {
                resultado += (bloqueActual[i] == '0') ? '1' : '0';
            }
        } else if (ceros > unos) {
            for (size_t i = 0; i < bloqueActual.length(); i += 2) {
                if (i + 1 < bloqueActual.length()) {
                    resultado += (bloqueActual[i + 1] == '0') ? '1' : '0';
                    resultado += (bloqueActual[i] == '0') ? '1' : '0';
                } else {
                    resultado += (bloqueActual[i] == '0') ? '1' : '0';
                }
            }
        } else {
            for (size_t i = 0; i < bloqueActual.length(); i += 3) {
                for (int k = 2; k >= 0; k--) {
                    if (i + k < bloqueActual.length()) {
                        resultado += (bloqueActual[i + k] == '0') ? '1' : '0';
                    }
                }
            }
        }
    }

    return resultado;
}

string metodo2String(const string& binario, int n) {
    string resultado = "";
    size_t longitud = binario.length();

    for (size_t i = 0; i < longitud; i += n) {
        string bloque = binario.substr(i, n);

        if (!bloque.empty()) {
            resultado += bloque[bloque.length() - 1];
            resultado += bloque.substr(0, bloque.length() - 1);
        }
    }

    return resultado;
}

void codificarConString(int n, int metodo, const string& archivoEntrada, const string& archivoSalida) {
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

        string binarioCodificado;
        if (metodo == 1) {
            binarioCodificado = metodo1String(binario, n);
        } else {
            binarioCodificado = metodo2String(binario, n);
        }

        string textoCodificado = binaryToString(binarioCodificado);

        FILE* salida = fopen(archivoSalida.c_str(), "wb");
        if (!salida) {
            throw "No se pudo crear el archivo de salida";
        }

        fwrite(textoCodificado.c_str(), 1, textoCodificado.length(), salida);
        fclose(salida);

        cout << "Archivo codificado exitosamente con string" << endl;

    } catch (const char* mensaje) {
        cerr << "Error: " << mensaje << endl;
    } catch (...) {
        cerr << "Error desconocido durante la codificación" << endl;
    }
}
