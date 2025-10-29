#ifndef SISTEMA_BANCARIO_H
#define SISTEMA_BANCARIO_H

#include <iostream>
#include <fstream>
#include <cstring>

struct Usuario {
    char cedula[20];
    char clave[20];
    double saldo;
};

// Funciones del sistema bancario
double stringADouble(const char* str);
void doubleAString(double valor, char* buffer, int tamBuffer);
void cargarUsuarios();
void guardarUsuarios();
void registrarTransaccion(const char* transaccion);
bool validarAdministrador(const char* clave);
void menuAdministrador();
void menuCliente();
void sistemaBancario();

#endif
