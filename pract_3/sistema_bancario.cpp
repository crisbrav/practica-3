#include "sistema_bancario.h"

using namespace std;

Usuario usuarios[100];
int numUsuarios = 0;

double stringADouble(const char* str) {
    double resultado = 0.0;
    double factor = 1.0;
    bool puntoEncontrado = false;
    int decimales = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            puntoEncontrado = true;
        } else if (str[i] >= '0' && str[i] <= '9') {
            if (puntoEncontrado) {
                decimales++;
                factor /= 10.0;
                resultado += (str[i] - '0') * factor;
            } else {
                resultado = resultado * 10.0 + (str[i] - '0');
            }
        }
    }
    return resultado;
}

void doubleAString(double valor, char* buffer, int tamBuffer) {
    int parteEntera = (int)valor;
    int parteDecimal = (int)((valor - parteEntera) * 100 + 0.5);

    // Convertir parte entera
    char enteraStr[20];
    int pos = 0;
    int temp = parteEntera;

    if (temp == 0) {
        enteraStr[pos++] = '0';
    } else {
        char tempStr[20];
        int tempPos = 0;
        while (temp > 0) {
            tempStr[tempPos++] = '0' + (temp % 10);
            temp /= 10;
        }
        for (int i = tempPos - 1; i >= 0; i--) {
            enteraStr[pos++] = tempStr[i];
        }
    }
    enteraStr[pos] = '\0';

    // Combinar
    int len = 0;
    for (int i = 0; enteraStr[i] != '\0' && len < tamBuffer - 1; i++) {
        buffer[len++] = enteraStr[i];
    }
    buffer[len++] = '.';

    // Parte decimal
    if (parteDecimal < 10) {
        buffer[len++] = '0';
        buffer[len++] = '0' + parteDecimal;
    } else {
        buffer[len++] = '0' + (parteDecimal / 10);
        buffer[len++] = '0' + (parteDecimal % 10);
    }
    buffer[len] = '\0';
}

void cargarUsuarios() {
    try {
        FILE* archivo = fopen("usuarios.txt", "r");
        if (!archivo) return;

        char linea[100];
        while (fgets(linea, sizeof(linea), archivo)) {
            linea[strcspn(linea, "\n")] = 0;

            char* token = strtok(linea, ",");
            if (token && numUsuarios < 100) {
                strcpy(usuarios[numUsuarios].cedula, token);

                token = strtok(NULL, ",");
                if (token) strcpy(usuarios[numUsuarios].clave, token);

                token = strtok(NULL, ",");
                if (token) {
                    usuarios[numUsuarios].saldo = stringADouble(token);
                }

                numUsuarios++;
            }
        }
        fclose(archivo);
    } catch (...) {
        cerr << "Error al cargar usuarios" << endl;
    }
}

void guardarUsuarios() {
    try {
        FILE* archivo = fopen("usuarios.txt", "w");
        if (!archivo) {
            throw "No se pudo guardar usuarios";
        }

        for (int i = 0; i < numUsuarios; i++) {
            char saldoStr[20];
            doubleAString(usuarios[i].saldo, saldoStr, sizeof(saldoStr));
            fprintf(archivo, "%s,%s,%s\n", usuarios[i].cedula, usuarios[i].clave, saldoStr);
        }
        fclose(archivo);
    } catch (const char* mensaje) {
        cerr << "Error: " << mensaje << endl;
    } catch (...) {
        cerr << "Error desconocido al guardar usuarios" << endl;
    }
}

void registrarTransaccion(const char* transaccion) {
    try {
        FILE* archivo = fopen("transacciones.txt", "a");
        if (!archivo) {
            throw "No se pudo registrar transacción";
        }

        fprintf(archivo, "%s\n", transaccion);
        fclose(archivo);
    } catch (const char* mensaje) {
        cerr << "Error: " << mensaje << endl;
    } catch (...) {
        cerr << "Error desconocido al registrar transacción" << endl;
    }
}

bool validarAdministrador(const char* clave) {
    try {
        FILE* archivo = fopen("sudo.txt", "r");
        if (!archivo) return false;

        char claveCorrecta[100];
        if (!fgets(claveCorrecta, sizeof(claveCorrecta), archivo)) {
            fclose(archivo);
            return false;
        }
        fclose(archivo);

        claveCorrecta[strcspn(claveCorrecta, "\n")] = 0;

        return strcmp(clave, claveCorrecta) == 0;
    } catch (...) {
        return false;
    }
}

void menuAdministrador() {
    cout << "\n=== MODO ADMINISTRADOR ===" << endl;
    int opcion;

    do {
        cout << "\n1. Registrar usuario" << endl;
        cout << "2. Listar usuarios" << endl;
        cout << "3. Salir" << endl;
        cout << "Seleccione: ";

        try {
            cin >> opcion;

            if (opcion == 1) {
                if (numUsuarios < 100) {
                    cout << "Cédula: ";
                    cin >> usuarios[numUsuarios].cedula;
                    cout << "Clave: ";
                    cin >> usuarios[numUsuarios].clave;
                    cout << "Saldo: ";
                    cin >> usuarios[numUsuarios].saldo;
                    numUsuarios++;
                    guardarUsuarios();
                    cout << "Usuario registrado." << endl;
                } else {
                    cout << "Límite de usuarios alcanzado." << endl;
                }
            } else if (opcion == 2) {
                cout << "\n=== USUARIOS ===" << endl;
                for (int i = 0; i < numUsuarios; i++) {
                    cout << "Cédula: " << usuarios[i].cedula << ", Saldo: $";
                    char saldoStr[20];
                    doubleAString(usuarios[i].saldo, saldoStr, sizeof(saldoStr));
                    cout << saldoStr << endl;
                }
            }
        } catch (...) {
            cerr << "Error en el menú de administrador" << endl;
            cin.clear();
            while (cin.get() != '\n');
        }
    } while (opcion != 3);
}

void menuCliente() {
    char cedula[20], clave[20];
    cout << "Cédula: ";
    cin >> cedula;
    cout << "Clave: ";
    cin >> clave;

    try {
        int usuarioIndex = -1;
        for (int i = 0; i < numUsuarios; i++) {
            if (strcmp(usuarios[i].cedula, cedula) == 0 && strcmp(usuarios[i].clave, clave) == 0) {
                usuarioIndex = i;
                break;
            }
        }

        if (usuarioIndex == -1) {
            cout << "Credenciales incorrectas." << endl;
            return;
        }

        // Cobrar acceso
        usuarios[usuarioIndex].saldo -= 1000;
        guardarUsuarios();

        char transaccion[100];
        sprintf(transaccion, "Acceso - %s - Costo: $1000", cedula);
        registrarTransaccion(transaccion);

        cout << "\nBienvenido " << cedula << endl;
        cout << "Costo de acceso: $1000" << endl;

        int opcion;
        do {
            cout << "\n1. Consultar saldo" << endl;
            cout << "2. Retirar dinero" << endl;
            cout << "3. Salir" << endl;
            cout << "Seleccione: ";
            cin >> opcion;

            if (opcion == 1) {
                cout << "Saldo: $";
                char saldoStr[20];
                doubleAString(usuarios[usuarioIndex].saldo, saldoStr, sizeof(saldoStr));
                cout << saldoStr << endl;

                sprintf(transaccion, "Consulta - %s", cedula);
                registrarTransaccion(transaccion);
            } else if (opcion == 2) {
                double cantidad;
                cout << "Cantidad a retirar: $";
                cin >> cantidad;

                if (cantidad <= usuarios[usuarioIndex].saldo) {
                    usuarios[usuarioIndex].saldo -= cantidad;
                    guardarUsuarios();
                    cout << "Retiro exitoso. Nuevo saldo: $";
                    char saldoStr[20];
                    doubleAString(usuarios[usuarioIndex].saldo, saldoStr, sizeof(saldoStr));
                    cout << saldoStr << endl;

                    sprintf(transaccion, "Retiro - %s - $%.2f", cedula, cantidad);
                    registrarTransaccion(transaccion);
                } else {
                    cout << "Saldo insuficiente." << endl;
                }
            }
        } while (opcion != 3);

    } catch (...) {
        cerr << "Error en el menú de cliente" << endl;
    }
}

void sistemaBancario() {
    cargarUsuarios();

    int opcion;
    do {
        cout << "\n=== SISTEMA BANCARIO ===" << endl;
        cout << "1. Administrador" << endl;
        cout << "2. Cliente" << endl;
        cout << "3. Salir" << endl;
        cout << "Seleccione: ";

        try {
            cin >> opcion;

            if (opcion == 1) {
                char clave[100];
                cout << "Clave de administrador: ";
                cin >> clave;

                if (validarAdministrador(clave)) {
                    menuAdministrador();
                } else {
                    cout << "Clave incorrecta." << endl;
                }
            } else if (opcion == 2) {
                menuCliente();
            }
        } catch (...) {
            cerr << "Error en el sistema bancario" << endl;
            cin.clear();
            while (cin.get() != '\n');
        }
    } while (opcion != 3);
}
