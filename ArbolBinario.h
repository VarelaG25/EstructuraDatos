#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;


// Estructura para almacenar datos de usuario
struct Usuario {
    char Username[100];
    int Edad;
    char Correo[100];
    char Contrasenia[100];
    char Nombre[100];
    char FechaNacimiento[100];
    char Genero[100];
    char RutaImagen[100];
    char ApellidoP[100];
    char ApellidoM[100];
    char FechaRegistro[100];
};

// Estructura para el nodo de usuario
struct NodoUsuario {
    NodoUsuario* izquierdo;
    NodoUsuario* derecho;
    Usuario usuario;

    // Constructor
    NodoUsuario(const Usuario& usr) : usuario(usr), izquierdo(nullptr), derecho(nullptr) {}
};


// Estructura para almacenar datos de usuario actual
struct UsuarioActual {
    char Username[100];
    int Edad;
    char Correo[100];
    char Contrasenia[100];
    char Nombre_U[100];
    char Apellido_P[100];
    char Apellido_M[100];
    char fechaNacimiento[100];
    char Genero[100];
    char RutaImagen[100];

    // Constructor
    UsuarioActual() {
        strcpy_s(Username, "");
        Edad = 0;
        strcpy_s(Correo, "");
        strcpy_s(Contrasenia, "");
        strcpy_s(Nombre_U, "");
        strcpy_s(Apellido_P, "");
        strcpy_s(Apellido_M, "");
        strcpy_s(fechaNacimiento, "");
        strcpy_s(Genero, "");
        strcpy_s(RutaImagen, "");
    }

    // Constructor con parámetros
    UsuarioActual(char _Usuario[258], char _Correo[258], char _Contrasenia[258], char _Nombre_U[258], 
        char _Apellido_P[258], char _Apellido_M[258], char _fechaNacimiento[258], char _Genero[258], 
        char _rutaFoto[258], int _Edad) 
    {
        strcpy_s(Username, _Usuario);
        strcpy_s(Correo, _Correo);
        strcpy_s(Contrasenia, _Contrasenia);
        strcpy_s(Nombre_U, _Nombre_U);
        strcpy_s(Apellido_P, _Apellido_P);
        strcpy_s(Apellido_M, _Apellido_M);
        strcpy_s(fechaNacimiento, _fechaNacimiento);
        strcpy_s(Genero, _Genero);
        strcpy_s(RutaImagen, _rutaFoto);
        Edad = _Edad;
    }
};

// Estructura para almacenar datos de usuario administrador
struct UsuarioAdmin {
    char usuarioAdmin[100];
    int EdadAdmin;
    char CorreoAdmin[100];
    char ContraseniaAdmin[100];
    char Nombre_UAdmin[100];
    char Apellido_PAdmin[100];
    char Apellido_MAdmin[100];
    char FechaNacimientoAdmin[100];
    char GeneroAdmin[100];
    char RutaFotoAdmin[100];

    // Constructor para inicializar los valores predeterminados
    UsuarioAdmin() {
        strcpy_s(usuarioAdmin, "Administrador");
        EdadAdmin = 25;
        strcpy_s(CorreoAdmin, "");
        strcpy_s(ContraseniaAdmin, "Contraseña");
        strcpy_s(Nombre_UAdmin, "");
        strcpy_s(Apellido_PAdmin, "");
        strcpy_s(Apellido_MAdmin, "");
        strcpy_s(FechaNacimientoAdmin, "");
        strcpy_s(GeneroAdmin, "");
        strcpy_s(RutaFotoAdmin, "img/abcd.bmp");
    }
};

void guardarNodosEnArchivo(NodoUsuario* nodo, ofstream& archivo) {
    if (nodo != nullptr) {
        // Guardar los datos del nodo actual en el archivo
        archivo << "Username: " << nodo->usuario.Username << endl;
        archivo << "Edad: " << nodo->usuario.Edad << endl;
        archivo << "Correo: " << nodo->usuario.Correo << endl;
        archivo << "Nombre: " << nodo->usuario.Nombre << endl;
        archivo << "Apellido Paterno: " << nodo->usuario.ApellidoP << endl;
        archivo << "Apellido Materno: " << nodo->usuario.ApellidoM << endl;
        archivo << "Fecha de Nacimiento: " << nodo->usuario.FechaNacimiento << endl;
        archivo << "Género: " << nodo->usuario.Genero << endl;
        archivo << "Ruta de la Imagen: " << nodo->usuario.RutaImagen << endl;
        archivo << endl;

        // Recursivamente guardar los nodos de los subárboles izquierdo y derecho
        guardarNodosEnArchivo(nodo->izquierdo, archivo);
        guardarNodosEnArchivo(nodo->derecho, archivo);
    }
}

// Función para crear el archivo de texto con los datos de los nodos registrados
void crearArchivoUsuarios(NodoUsuario* raiz, const char* nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo para escritura." << endl;
        return;
    }

    // Llamar a la función para guardar los nodos en el archivo
    guardarNodosEnArchivo(raiz, archivo);

    archivo.close();
}

class ArbolBinario {
private:
    NodoUsuario* raiz;

    // Función privada para insertar de forma recursiva
    NodoUsuario* insertarRecursivo(NodoUsuario* nodo, const Usuario& nuevoUsuario) {
        if (nodo == nullptr) {
            return new NodoUsuario(nuevoUsuario);
        }

        if (strcmp(nuevoUsuario.Username, nodo->usuario.Username) < 0) {
            nodo->izquierdo = insertarRecursivo(nodo->izquierdo, nuevoUsuario);
        }
        else if (strcmp(nuevoUsuario.Username, nodo->usuario.Username) > 0) {
            nodo->derecho = insertarRecursivo(nodo->derecho, nuevoUsuario);
        }

        return nodo;
    }

    // Función privada para buscar un usuario de forma recursiva
    NodoUsuario* buscarRecursivo(NodoUsuario* nodo, const char* username) {
        if (nodo == nullptr || strcmp(username, nodo->usuario.Username) == 0) {
            return nodo;
        }

        if (strcmp(username, nodo->usuario.Username) < 0) {
            return buscarRecursivo(nodo->izquierdo, username);
        }

        return buscarRecursivo(nodo->derecho, username);
    }

    // Función privada para liberar la memoria del árbol
    void liberarMemoria(NodoUsuario* nodo) {
        if (nodo != nullptr) {
            liberarMemoria(nodo->izquierdo);
            liberarMemoria(nodo->derecho);
            delete nodo;
        }
    }

public:
    // Constructor
    ArbolBinario() : raiz(nullptr) {}

    // Destructor
    ~ArbolBinario() {
        liberarMemoria(raiz);
    }

    // Método público para insertar un nuevo usuario
    void insertar(const Usuario& nuevoUsuario) {
        raiz = insertarRecursivo(raiz, nuevoUsuario);
    }

    // Método público para buscar un usuario por su nombre de usuario
    NodoUsuario* buscar(const char* username) {
        return buscarRecursivo(raiz, username);
    }

    // Método público para guardar el árbol en un archivo binario
    void guardar(const char* filename) {
        ofstream file(filename, ios::binary);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo para escritura." << endl;
            MessageBoxA(NULL, "Error al abrir el archivo para escritura.", "Error", MB_OK | MB_ICONERROR);
            return;
        }
        guardarRecursivo(raiz, file);
        file.close();
    }

    // Método privado para guardar de forma recursiva
    void guardarRecursivo(NodoUsuario* nodo, ofstream& file) {
        if (nodo != nullptr) {
            // por estas
            file.write(reinterpret_cast<char*>(&nodo->usuario), sizeof(nodo->usuario));
            guardarRecursivo(nodo->izquierdo, file);
            guardarRecursivo(nodo->derecho, file);
        }
    }

    // Método público para cargar el árbol desde un archivo binario
    void cargar(const char* filename) {
        ifstream file(filename, ios::binary);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo para lectura." << endl;
            return;
        }
        Usuario usuario;
        while (file.read(reinterpret_cast<char*>(&usuario), sizeof(Usuario))) {
            insertar(usuario);
        }
        file.close();
    }

    // Función para obtener el primer usuario en el árbol
    NodoUsuario* getPrimerUsuario() {
        NodoUsuario* usuario = raiz;
        while (usuario != nullptr && usuario->izquierdo != nullptr) {
            usuario = usuario->izquierdo;
        }
        return usuario;
    }

    // Función para obtener el siguiente usuario en el árbol después de un usuario dado
    NodoUsuario* getSiguienteUsuario(NodoUsuario* usuarioActual) {
        if (usuarioActual->derecho != nullptr) {
            usuarioActual = usuarioActual->derecho;
            while (usuarioActual->izquierdo != nullptr) {
                usuarioActual = usuarioActual->izquierdo;
            }
            return usuarioActual;
        }

        NodoUsuario* siguiente = nullptr;
        NodoUsuario* temp = raiz;

        while (temp != nullptr) {
            if (strcmp(usuarioActual->usuario.Username, temp->usuario.Username) < 0) {
                siguiente = temp;
                temp = temp->izquierdo;
            }
            else if (strcmp(usuarioActual->usuario.Username, temp->usuario.Username) > 0) {
                temp = temp->derecho;
            }
            else {
                break;
            }
        }

        return siguiente;
    }

    // Función para escribir los datos del árbol en un archivo de texto
    void escribirEnArchivoTexto(const char* nombreArchivo) {
        ofstream archivo(nombreArchivo);
        if (archivo.is_open()) {
            escribirRecursivo(archivo, raiz);
            archivo.close();
            cout << "Datos del árbol escritos en el archivo de texto: " << nombreArchivo << endl;
        }
        else {
            cout << "No se pudo abrir el archivo de texto para escritura." << endl;
        }
    }

    // Función privada para escribir de forma recursiva
    void escribirRecursivo(ofstream& archivo, NodoUsuario* nodo) {
        if (nodo != nullptr) {
            escribirRecursivo(archivo, nodo->izquierdo);
            archivo << "Usuario: " << nodo->usuario.Username << endl;
            archivo << "Correo: " << nodo->usuario.Correo << endl;
            archivo << "Contraseña: " << nodo->usuario.Contrasenia << endl;
            archivo << "Nombre: " << nodo->usuario.Nombre << endl;
            archivo << "Apellido Paterno: " << nodo->usuario.ApellidoP << endl;
            archivo << "Apellido Materno: " << nodo->usuario.ApellidoM << endl;
            archivo << "Fecha de Nacimiento: " << nodo->usuario.FechaNacimiento << endl;
            archivo << "Género: " << nodo->usuario.Genero << endl;
            archivo << "Ruta de Foto: " << nodo->usuario.RutaImagen << endl;
            archivo << "-------------------------" << endl;
            escribirRecursivo(archivo, nodo->derecho);
        }
    }

    bool eliminarUsuario(const char* username) {
        bool eliminado = false;
        raiz = eliminarNodo(raiz, username, eliminado);
        return eliminado;
    }

    NodoUsuario* eliminarNodo(NodoUsuario* nodo, const char* username, bool& eliminado) {
        if (nodo == nullptr) return nodo;

        if (strcmp(username, nodo->usuario.Username) < 0) {
            nodo->izquierdo = eliminarNodo(nodo->izquierdo, username, eliminado);
        }
        else if (strcmp(username, nodo->usuario.Username) > 0) {
            nodo->derecho = eliminarNodo(nodo->derecho, username, eliminado);
        }
        else {
            eliminado = true;
            if (nodo->izquierdo == nullptr) {
                NodoUsuario* temp = nodo->derecho;
                delete nodo;
                return temp;
            }
            else if (nodo->derecho == nullptr) {
                NodoUsuario* temp = nodo->izquierdo;
                delete nodo;
                return temp;
            }

            NodoUsuario* temp = minValueNode(nodo->derecho);
            nodo->usuario = temp->usuario;
            nodo->derecho = eliminarNodo(nodo->derecho, temp->usuario.Username, eliminado);
        }
        return nodo;
    }

    NodoUsuario* minValueNode(NodoUsuario* nodo) {
        NodoUsuario* actual = nodo;
        while (actual && actual->izquierdo != nullptr)
            actual = actual->izquierdo;
        return actual;
    }

    
};

void guardarUsuarioActual(const UsuarioActual& usuario, const char* filename) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo para escritura." << endl;
        MessageBoxA(NULL, "Error al abrir el archivo para escritura.", "Error", MB_OK | MB_ICONERROR);
        return;
    }
    file.write(reinterpret_cast<const char*>(&usuario), sizeof(UsuarioActual));
    file.close();
}

bool cargarUsuarioActual(UsuarioActual& usuario, const char* filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        return false; // No mostrar error si el archivo no existe
    }
    if (!file.read(reinterpret_cast<char*>(&usuario), sizeof(UsuarioActual))) {
        file.close();
        return false;
    }
    file.close();
    return true;
}

void cargarImagenUsuario(HWND hwnd, const char* rutaImagen) {
    HBITMAP hBitmap = (HBITMAP)LoadImageA(NULL, rutaImagen, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
    if (hBitmap) {
        SendMessage(GetDlgItem(hwnd, US_ActualImage), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
    }
}

