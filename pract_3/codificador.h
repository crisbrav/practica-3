#ifndef CODIFICADOR_H
#define CODIFICADOR_H

#include <iostream>
#include <fstream>
#include <cstring>

// Funciones para string
std::string stringToBinary(const std::string& texto);
std::string binaryToString(const std::string& binario);
void contarBitsString(const std::string& bloque, int& ceros, int& unos);
std::string metodo1String(const std::string& binario, unsigned long n);
std::string metodo2String(const std::string& binario, unsigned long n);
void codificarConString(unsigned long n, int metodo, const std::string& archivoEntrada, const std::string& archivoSalida);

// Funciones de validación
bool validarSemilla(unsigned long semilla);
bool validarMetodo(int metodo);
bool validarArchivo(const std::string& nombreArchivo);

#endif
