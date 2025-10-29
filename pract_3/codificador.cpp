#include "codificador.h"

using namespace std;


bool validarSemilla(unsigned long semilla) {
    try {
        if (semilla <= 0) {
            throw "La semilla debe ser un número positivo";
        }
        if (semilla > 100) {
            throw "La semilla no puede ser mayor a 100";
        }
        return true;
    } catch (const char* mensaje) {
        cerr << "Error validacion semilla: " << mensaje << endl;
        return false;
    }
}

bool validarMetodo(int metodo) {
    try {
        if (metodo != 1 && metodo != 2) {
            throw "El metodo debe ser 1 o 2";
        }
        return true;
    } catch (const char* mensaje) {
        cerr << "Error validacion metodo: " << mensaje << endl;
        return false;
    }
}

bool validarArchivo(const string& nombreArchivo) {
    try {
        FILE* archivo = fopen(nombreArchivo.c_str(), "rb");
        if (!archivo) {
            throw "No se pudo abrir el archivo para validacion";
        }
        fclose(archivo);
        return true;
    } catch (const char* mensaje) {
        cerr << "Error validacion archivo '" << nombreArchivo << "': " << mensaje << endl;
        return false;
    }
}

string stringToBinary(const string& texto) {
    try {
        if (texto.empty()) {
            throw "Texto vaceo en conversion a binario";
        }

        string binario = "";
        for (unsigned long i = 0; i < texto.length(); i++) {
            char c = texto[i];
            for (int j = 7; j >= 0; j--) {
                binario += ((c >> j) & 1) ? '1' : '0';
            }
        }
        return binario;
    } catch (const char* mensaje) {
        throw string("Error en stringToBinary: ") + mensaje;
    } catch (...) {
        throw "Error desconocido en stringToBinary";
    }
}

string binaryToString(const string& binario) {
    try {
        if (binario.empty()) {
            throw "Binario vaceo en conversion a texto";
        }

        if (binario.length() % 8 != 0) {
            throw "Longitud de binario no es múltiplo de 8";
        }

        string resultado = "";
        for (unsigned long i = 0; i < binario.length(); i += 8) {
            if (i + 8 <= binario.length()) {
                string byteStr = binario.substr(i, 8);
                char c = 0;
                for (int j = 0; j < 8; j++) {
                    if (byteStr[j] != '0' && byteStr[j] != '1') {
                        throw "Carecter invelido en cadena binaria";
                    }
                    c = (c << 1) | (byteStr[j] - '0');
                }
                resultado += c;
            }
        }
        return resultado;
    } catch (const char* mensaje) {
        throw string("Error en binaryToString: ") + mensaje;
    } catch (...) {
        throw "Error desconocido en binaryToString";
    }
}

void contarBitsString(const string& bloque, int& ceros, int& unos) {
    try {
        ceros = 0;
        unos = 0;
        for (unsigned long i = 0; i < bloque.length(); i++) {
            if (bloque[i] == '0') ceros++;
            else if (bloque[i] == '1') unos++;
            else throw "Carecter invelido en bloque binario";
        }
    } catch (const char* mensaje) {
        throw string("Error en contarBitsString: ") + mensaje;
    }
}

string metodo1String(const string& binario, unsigned long n) {
    try {
        if (binario.empty()) {
            throw "Binario vaceo en metodo 1";
        }

        if (!validarSemilla(n)) {
            throw "Semilla invelida para metodo 1";
        }

        string resultado = "";
        unsigned long longitud = binario.length();

        unsigned long numBloques = (longitud + n - 1) / n;

        string* bloquesCodificados = new string[numBloques];

        string primerBloque = "";
        for (unsigned long i = 0; i < n && i < longitud; i++) {
            primerBloque += (binario[i] == '0') ? '1' : '0';
        }
        bloquesCodificados[0] = primerBloque;
        resultado += primerBloque;

        for (unsigned long bloque = 1; bloque < numBloques; bloque++) {
            unsigned long inicioActual = bloque * n;

            string bloqueAnteriorCodificado = bloquesCodificados[bloque - 1];

            int ceros = 0, unos = 0;
            for (unsigned long i = 0; i < bloqueAnteriorCodificado.length(); i++) {
                if (bloqueAnteriorCodificado[i] == '0') ceros++;
                else if (bloqueAnteriorCodificado[i] == '1') unos++;
            }

            string bloqueActualOriginal = "";
            for (unsigned long i = 0; i < n && (inicioActual + i) < longitud; i++) {
                bloqueActualOriginal += binario[inicioActual + i];
            }

            while (bloqueActualOriginal.length() < n) {
                bloqueActualOriginal += '0';
            }

            string bloqueActualCodificado = bloqueActualOriginal;

            if (ceros == unos) {
                for (unsigned long i = 0; i < bloqueActualCodificado.length(); i++) {
                    bloqueActualCodificado[i] = (bloqueActualOriginal[i] == '0') ? '1' : '0';
                }
            } else if (ceros > unos) {
                for (unsigned long i = 1; i < bloqueActualCodificado.length(); i += 2) {
                    bloqueActualCodificado[i] = (bloqueActualOriginal[i] == '0') ? '1' : '0';
                }
            } else {
                for (unsigned long i = 2; i < bloqueActualCodificado.length(); i += 3) {
                    bloqueActualCodificado[i] = (bloqueActualOriginal[i] == '0') ? '1' : '0';
                }
            }

            bloquesCodificados[bloque] = bloqueActualCodificado;
            resultado += bloqueActualCodificado;
        }

        if (resultado.length() > longitud) {
            resultado = resultado.substr(0, longitud);
        }

        delete[] bloquesCodificados;
        return resultado;

    } catch (const char* mensaje) {
        throw string("Error en metodo1String: ") + mensaje;
    } catch (...) {
        throw "Error desconocido en metodo1String";
    }
}

string metodo2String(const string& binario, unsigned long n) {
    try {
        if (binario.empty()) {
            throw "Binario vaceo en metodo 2";
        }

        if (!validarSemilla(n)) {
            throw "Semilla invelida para metodo 2";
        }

        string resultado = "";
        unsigned long longitud = binario.length();

        unsigned long bitsProcesados = 0;

        for (unsigned long i = 0; i < longitud && bitsProcesados < longitud; i += n) {
            string bloque = "";
            unsigned long bitsEnBloque = 0;

            for (unsigned long j = 0; j < n && (i + j) < longitud; j++) {
                bloque += binario[i + j];
                bitsEnBloque++;
                bitsProcesados++;
            }

            if (bitsEnBloque >= 2) {
                string bloqueRotado = "";
                bloqueRotado += bloque[bitsEnBloque - 1];
                bloqueRotado += bloque.substr(0, bitsEnBloque - 1);
                resultado += bloqueRotado;
            } else if (bitsEnBloque == 1) {
                resultado += bloque;
            }
        }

        return resultado;

    } catch (const char* mensaje) {
        throw string("Error en metodo2String: ") + mensaje;
    } catch (...) {
        throw "Error desconocido en metodo2String";
    }
}

void codificarConString(unsigned long n, int metodo, const string& archivoEntrada, const string& archivoSalida) {
    try {
        if (!validarSemilla(n)) {
            throw "Semilla invelida";
        }
        if (!validarMetodo(metodo)) {
            throw "Metodo invelido";
        }
        if (!validarArchivo(archivoEntrada)) {
            throw "Archivo de entrada invelido";
        }

        FILE* archivo = fopen(archivoEntrada.c_str(), "rb");
        if (!archivo) {
            throw string("No se pudo abrir el archivo de entrada: ") + archivoEntrada;
        }

        fseek(archivo, 0, SEEK_END);
        long tam = ftell(archivo);
        fseek(archivo, 0, SEEK_SET);

        if (tam <= 0) {
            fclose(archivo);
            throw "El archivo de entrada este vaceo";
        }

        char* buffer = new char[tam + 1];
        unsigned long leidos = fread(buffer, 1, tam, archivo);
        if (leidos != (unsigned long)tam) {
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
            throw string("No se pudo crear el archivo de salida: ") + archivoSalida;
        }

        unsigned long escritos = fwrite(textoCodificado.c_str(), 1, textoCodificado.length(), salida);
        if (escritos != textoCodificado.length()) {
            fclose(salida);
            throw "Error al escribir el archivo de salida";
        }

        fclose(salida);

        cout << " Archivo codificado exitosamente" << endl;
        cout << "  Semilla: " << n << " ("
             << (n % 2 == 0 ? "PAR" : "IMPAR") << ")" << endl;
        cout << "  Metodo: " << metodo << endl;
        cout << "  Entrada: " << archivoEntrada  << endl;
        cout << "  Salida: " << archivoSalida  << endl;

    } catch (const char* mensaje) {
        cerr << " Error durante codificacion: " << mensaje << endl;
    } catch (const string& mensaje) {
        cerr << " Error durante codificacion: " << mensaje << endl;
    } catch (...) {
        cerr << " Error desconocido durante la codificacion" << endl;
    }
}
