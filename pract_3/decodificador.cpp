#include "decodificador.h"
#include "codificador.h"

using namespace std;

string metodo1Decodificar(const string& binario, unsigned long n) {
    try {
        if (binario.empty()) {
            throw "Binario vacio en decodificacion metodo 1";
        }

        if (!validarSemilla(n)) {
            throw "Semilla invalida para decodificacion metodo 1";
        }

        string resultado = "";
        unsigned long longitud = binario.length();

        // Calcular número total de bloques
        unsigned long numBloques = (longitud + n - 1) / n;

        // Vector para almacenar bloques CODIFICADOS (los que recibimos)
        string* bloquesCodificados = new string[numBloques];

        // Separar en bloques codificados
        for (unsigned long i = 0; i < numBloques; i++) {
            string bloque = "";
            for (unsigned long j = 0; j < n; j++) {
                unsigned long pos = i * n + j;
                if (pos < longitud) {
                    bloque += binario[pos];
                } else {
                    bloque += '0'; // Padding
                }
            }
            bloquesCodificados[i] = bloque;
        }

        string primerBloqueDecodificado = "";
        for (unsigned long i = 0; i < bloquesCodificados[0].length(); i++) {
            primerBloqueDecodificado += (bloquesCodificados[0][i] == '0') ? '1' : '0';
        }
        resultado += primerBloqueDecodificado;

        for (unsigned long bloque = 1; bloque < numBloques; bloque++) {
            string bloqueAnteriorCodificado = bloquesCodificados[bloque - 1];

            int ceros = 0, unos = 0;
            for (unsigned long i = 0; i < bloqueAnteriorCodificado.length(); i++) {
                if (bloqueAnteriorCodificado[i] == '0') ceros++;
                else if (bloqueAnteriorCodificado[i] == '1') unos++;
            }

            string bloqueActualCodificado = bloquesCodificados[bloque];
            string bloqueActualDecodificado = bloqueActualCodificado;

            if (ceros == unos) {
                for (unsigned long i = 0; i < bloqueActualDecodificado.length(); i++) {
                    bloqueActualDecodificado[i] = (bloqueActualCodificado[i] == '0') ? '1' : '0';
                }
            } else if (ceros > unos) {
                for (unsigned long i = 1; i < bloqueActualDecodificado.length(); i += 2) {
                    bloqueActualDecodificado[i] = (bloqueActualCodificado[i] == '0') ? '1' : '0';
                }
            } else {
                for (unsigned long i = 2; i < bloqueActualDecodificado.length(); i += 3) {
                    bloqueActualDecodificado[i] = (bloqueActualCodificado[i] == '0') ? '1' : '0';
                }
            }

            resultado += bloqueActualDecodificado;
        }

        if (resultado.length() > longitud) {
            resultado = resultado.substr(0, longitud);
        }

        delete[] bloquesCodificados;
        return resultado;

    } catch (const char* mensaje) {
        throw string("Error en metodo1Decodificar: ") + mensaje;
    } catch (...) {
        throw "Error desconocido en metodo1Decodificar";
    }
}


string metodo2Decodificar(const string& binario, unsigned long n) {
    try {
        if (binario.empty()) {
            throw "Binario vacio en decodificacion metodo 2";
        }

        if (!validarSemilla(n)) {
            throw "Semilla invalida para decodificacion metodo 2";
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
                string bloqueDecodificado = bloque.substr(1, bitsEnBloque - 1);
                bloqueDecodificado += bloque[0];
                resultado += bloqueDecodificado;
            } else if (bitsEnBloque == 1) {
                resultado += bloque;
            }
        }

        return resultado;

    } catch (const char* mensaje) {
        throw string("Error en metodo2Decodificar: ") + mensaje;
    } catch (...) {
        throw "Error desconocido en metodo2Decodificar";
    }
}

void decodificarConString(unsigned long n, int metodo, const string& archivoEntrada, const string& archivoSalida) {
    try {
        // VALIDACIONES COMPLETAS
        if (!validarSemilla(n)) {
            throw "Semilla invalida";
        }
        if (!validarMetodo(metodo)) {
            throw "Metodo invalido";
        }
        if (!validarArchivo(archivoEntrada)) {
            throw "Archivo de entrada invalido";
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
            throw "El archivo de entrada esta vacio";
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
        string binarioDecodificado;

        if (metodo == 1) {
            binarioDecodificado = metodo1Decodificar(binario, n);
        } else {
            binarioDecodificado = metodo2Decodificar(binario, n);
        }

        string textoDecodificado = binaryToString(binarioDecodificado);

        FILE* salida = fopen(archivoSalida.c_str(), "wb");
        if (!salida) {
            throw string("No se pudo crear el archivo de salida: ") + archivoSalida;
        }

        unsigned long escritos = fwrite(textoDecodificado.c_str(), 1, textoDecodificado.length(), salida);
        if (escritos != textoDecodificado.length()) {
            fclose(salida);
            throw "Error al escribir el archivo de salida";
        }

        fclose(salida);

        cout << " Archivo decodificado exitosamente" << endl;
        cout << "  Semilla: " << n << " ("
             << (n % 2 == 0 ? "PAR" : "IMPAR") << ")" << endl;
        cout << "  Metodo: " << metodo << endl;
        cout << "  Entrada: " << archivoEntrada << endl;
        cout << "  Salida: " << archivoSalida << endl;

        if (textoDecodificado.length() == 0) {
            cerr << "Archivo decodificado esta vacio" << endl;
        }

    } catch (const char* mensaje) {
        cerr << " Error durante decodificacion: " << mensaje << endl;
    } catch (const string& mensaje) {
        cerr << " Error durante decodificacion: " << mensaje << endl;
    } catch (...) {
        cerr << " Error desconocido durante la decodificacion" << endl;
    }
}
