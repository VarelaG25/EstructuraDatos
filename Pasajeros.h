#pragma once

#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

// Estructura para almacenar datos de pasajero
struct Pasajero {
    char Nombre[100];
    char Apellido_P[100];
    char Apellido_M[100];
    char FechaNacimiento[100];
    char Genero[100];
    char Nacionalidad[100];
};

// Estructura para el nodo de pasajero
struct NodoPasajero {
    Pasajero pasajero;
    NodoPasajero* siguiente;
    NodoPasajero* anterior;

    // Constructor
    NodoPasajero(const Pasajero& p) : pasajero(p), siguiente(nullptr), anterior(nullptr) {}
};

class ListaDoblePasajero {
public:
    NodoPasajero* cabeza;
    NodoPasajero* cola;

    ListaDoblePasajero() : cabeza(nullptr), cola(nullptr) {}

    void agregarAlInicio(const Pasajero& p) {
        NodoPasajero* nuevo = new NodoPasajero(p);
        if (!cabeza) {
            cabeza = nuevo;
            cola = nuevo;
        }
        else {
            nuevo->siguiente = cabeza;
            cabeza->anterior = nuevo;
            cabeza = nuevo;
        }
    }

    void guardar(const char* filename) {
        ofstream file(filename, ios::out | ios::binary | ios::trunc);
        if (!file.is_open()) {
            cerr << "Error: no se pudo abrir el archivo de pasajeros." << endl;
            return;
        }
        NodoPasajero* temp = cabeza;
        while (temp != nullptr) {
            file.write(reinterpret_cast<char*>(&temp->pasajero), sizeof(Pasajero));
            temp = temp->siguiente;
        }
        file.close();
        cout << "Lista de pasajeros guardada exitosamente en el archivo '" << filename << "'." << endl;
    }

    void cargar(const char* filename) {
        ifstream file(filename, ios::in | ios::binary);
        if (!file.is_open()) {
            cerr << "Error: no se pudo abrir el archivo de pasajeros." << endl;
            return;
        }
        Pasajero pasajero;
        while (file.read(reinterpret_cast<char*>(&pasajero), sizeof(Pasajero))) {
            agregarAlInicio(pasajero);
        }
        file.close();
        cout << "Lista de pasajeros cargada exitosamente desde el archivo '" << filename << "'." << endl;
    }

    NodoPasajero* buscar(const char* nombre) {
        NodoPasajero* actual = cabeza;
        while (actual != nullptr) {
            if (strcmp(actual->pasajero.Nombre, nombre) == 0) {
                return actual;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }

    void eliminarNodo(NodoPasajero* nodo) {
        if (!nodo) {
            cerr << "Error: el nodo a eliminar es nulo." << endl;
            return;
        }
        // Si el nodo a eliminar es la cabeza
        if (nodo == cabeza) {
            cabeza = cabeza->siguiente;
            if (cabeza) {
                cabeza->anterior = nullptr;
            }
            else {
                cola = nullptr;
            }
            delete nodo;
            return;
        }
        // Si el nodo a eliminar es la cola
        if (nodo == cola) {
            cola = cola->anterior;
            cola->siguiente = nullptr;
            delete nodo;
            return;
        }
        // Si el nodo a eliminar está en medio de la lista
        nodo->anterior->siguiente = nodo->siguiente;
        nodo->siguiente->anterior = nodo->anterior;
        delete nodo;
    }

    void modificarNodo(NodoPasajero* nodo, const Pasajero& nuevoPasajero) {
        if (!nodo) {
            cerr << "Error: el nodo a modificar es nulo." << endl;
            return;
        }
        // Se actualizan los datos del pasajero en el nodo
        nodo->pasajero = nuevoPasajero;
    }
};