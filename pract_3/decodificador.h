#ifndef DECODIFICADOR_H
#define DECODIFICADOR_H

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

string metodo1Decodificar(const string& binario, int n);
string metodo2Decodificar(const string& binario, int n);
void decodificarConString(int n, int metodo, const string& archivoEntrada, const string& archivoSalida);


#endif // DECODIFICADOR_H
