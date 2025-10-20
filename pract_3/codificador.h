#ifndef CODIFICADOR_H
#define CODIFICADOR_H

#include <iostream>
#include <fstream>
#include <cstring>

std::string stringToBinary(const std::string& texto);
std::string binaryToString(const std::string& binario);
void contarBitsString(const std::string& bloque, int& ceros, int& unos);
std::string metodo1String(const std::string& binario, int n);
std::string metodo2String(const std::string& binario, int n);
void codificarConString(int n, int metodo, const std::string& archivoEntrada, const std::string& archivoSalida);


#endif // CODIFICADOR_H
