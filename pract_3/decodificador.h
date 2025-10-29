#ifndef DECODIFICADOR_H
#define DECODIFICADOR_H

#include <iostream>
#include <fstream>
#include <cstring>

// Funciones de decodificación
std::string metodo1Decodificar(const std::string& binario, int n);
std::string metodo2Decodificar(const std::string& binario, int n);
void decodificarConString(unsigned long n, int metodo, const std::string& archivoEntrada, const std::string& archivoSalida);

#endif
