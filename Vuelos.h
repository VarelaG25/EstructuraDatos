#include <iostream>
#include <windows.h>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <Commctrl.h>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

struct Vuelo {
    char Origen[100];
    char Destino[100];
    char FechaOrigen[100];
    char HoraOrigen[100];
    char FechaDestino[100];
    char HoraDestino[100];
    int NumVuelo;
    char TipoAvion[100];
    int NumAsientosTot;
    int NumAsientoTu;
    int NumAsientoEj;
    int Menores;
    int Mayores;
    int CostoTu;
    int CostoEj;
    char Estatus[100] = "NoConfirmado";
    char FechaRegistro[100];
    int asientosMenores;
    int asientosMayores;
};

struct NodoVuelo {
    Vuelo vuelo;
    NodoVuelo* siguiente;
    NodoVuelo* anterior;

    NodoVuelo(const Vuelo& v) : vuelo(v), siguiente(nullptr), anterior(nullptr) {}
};


class ListaDobleVuelo {
public:
    NodoVuelo* cabeza;
    NodoVuelo* cola;

    // Constructor
    ListaDobleVuelo() : cabeza(nullptr), cola(nullptr) {}

    // Función para agregar un elemento al final de la lista 
    void agregarAlFinal(const Vuelo& v) {
        NodoVuelo* nuevo = new NodoVuelo(v);
        if (!cabeza) {
            cabeza = nuevo;
            cola = nuevo;
        }
        else {
            cola->siguiente = nuevo;
            nuevo->anterior = cola;
            cola = nuevo;
        }
    }



    // Función para guardar la lista en un archivo binario
    void guardar(const char* filename) {
        ofstream file(filename, ios::out | ios::binary | ios::trunc);
        if (!file.is_open()) {
            cerr << "Error: no se pudo abrir el archivo de vuelos." << endl;
            return;
        }
        NodoVuelo* temp = cabeza;
        while (temp != nullptr) {
            file.write(reinterpret_cast<char*>(&temp->vuelo), sizeof(Vuelo));
            temp = temp->siguiente;
        }
        file.close();
        cout << "Lista de vuelos guardada exitosamente en el archivo '" << filename << "'." << endl;
    }

    void cargar(const char* filename) {
        ifstream file(filename, ios::in | ios::binary);
        if (!file.is_open()) {
            cerr << "Error: no se pudo abrir el archivo de vuelos." << endl;
            return;
        }
        Vuelo vuelo;
        while (file.read(reinterpret_cast<char*>(&vuelo), sizeof(Vuelo))) {
            agregarAlFinal(vuelo);
        }
        file.close();
        cout << "Lista de vuelos cargada exitosamente desde el archivo '" << filename << "'." << endl;
    }

    NodoVuelo* buscar(int numVuelo) {
        NodoVuelo* actual = cabeza;
        while (actual != nullptr) {
            if (actual->vuelo.NumVuelo == numVuelo) {
                return actual;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }

    void eliminarNodo(NodoVuelo* nodo) {
        if (!nodo) {
            cerr << "Error: el nodo a eliminar es nulo." << endl;
            return;
        }
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
        if (nodo == cola) {
            cola = cola->anterior;
            cola->siguiente = nullptr;
            delete nodo;
            return;
        }
        nodo->anterior->siguiente = nodo->siguiente;
        nodo->siguiente->anterior = nodo->anterior;
        delete nodo;
    }


    void modificarNodo(NodoVuelo* nodo, const Vuelo& nuevoVuelo) {
        if (!nodo) {
            cerr << "Error: el nodo a modificar es nulo." << endl;
            return;
        }
        nodo->vuelo = nuevoVuelo;
    }

    void quicksort(NodoVuelo* izq, NodoVuelo* der) {
        if (izq == nullptr || der == nullptr || izq == der || izq->anterior == der) return;

        NodoVuelo* pivote = izq;
        NodoVuelo* i = izq;
        NodoVuelo* j = der;

        while (i != j) {
            while (j != i && j->vuelo.NumVuelo >= pivote->vuelo.NumVuelo) {
                j = j->anterior;
            }
            while (i != j && i->vuelo.NumVuelo <= pivote->vuelo.NumVuelo) {
                i = i->siguiente;
            }
            if (i != j) {
                Vuelo temp = i->vuelo;
                i->vuelo = j->vuelo;
                j->vuelo = temp;
            }
        }

        if (izq != j) {
            Vuelo temp = izq->vuelo;
            izq->vuelo = j->vuelo;
            j->vuelo = temp;
        }

        quicksort(izq, j->anterior);
        quicksort(j->siguiente, der);
    }

    void ordenarPorQuicksort() {
        if (cabeza == nullptr || cabeza->siguiente == nullptr) return;
        quicksort(cabeza, cola);
    }

    NodoVuelo* buscarBinario(int numVuelo) {
        vector<Vuelo> vuelos;
        NodoVuelo* temp = cabeza;
        while (temp != nullptr) {
            vuelos.push_back(temp->vuelo);
            temp = temp->siguiente;
        }

        int izquierda = 0;
        int derecha = vuelos.size() - 1;
        while (izquierda <= derecha) {
            int medio = izquierda + (derecha - izquierda) / 2;
            if (vuelos[medio].NumVuelo == numVuelo) {
                // Encontrado
                temp = cabeza;
                while (temp != nullptr) {
                    if (temp->vuelo.NumVuelo == numVuelo) {
                        return temp;
                    }
                    temp = temp->siguiente;
                }
            }
            if (vuelos[medio].NumVuelo < numVuelo) {
                izquierda = medio + 1;
            }
            else {
                derecha = medio - 1;
            }
        }

        return nullptr;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////// ASIENTOS 

struct Asiento {
    char fila;
    int numero;
    bool disponible;
    int numAvion; // Añadido para coincidir con tu inicializar_asientos

    Asiento(char f = '\0', int n = 0, int numA = 0) : fila(f), numero(n), disponible(true), numAvion(numA) {}
};

struct NodoAsiento {
    Asiento asiento;
    NodoAsiento* siguiente;
    NodoAsiento* anterior;

    NodoAsiento(Asiento a) : asiento(a), siguiente(nullptr), anterior(nullptr) {}
};

class ListaDobleAsiento {
public:
    NodoAsiento* cabeza;
    NodoAsiento* cola;

    ListaDobleAsiento() : cabeza(nullptr), cola(nullptr) {}

    void agregarAlFinal(Asiento asiento) {

        NodoAsiento* nuevo = new NodoAsiento(asiento);
        if (!cabeza) {
            cabeza = nuevo;
            cola = nuevo;
        }
        else {
            cola->siguiente = nuevo;
            nuevo->anterior = cola;
            cola = nuevo;
        }

    }

    void inicializar_asientos(int na) {
        for (char fila = 'A'; fila <= 'Z'; fila++) {
            for (int numero = 1; numero <= 6; numero++) {
                agregarAlFinal(Asiento(fila, numero, na));
            }
        }

    }

    void ocultar_asiento(char fila, int numero, int vuelo) {
        NodoAsiento* encontrado = busquedaBinaria(fila, numero, vuelo);
        if (encontrado) {
            encontrado->asiento.disponible = false;
        }
        else {
            // Maneja el caso donde no se encuentra el asiento
           // MessageBoxA(hwnd, "Asiento no encontrado.", "Error", MB_OK | MB_ICONERROR);
        }
    }

    void mostrar_asiento(char fila, int numero, int vuelo) {
        NodoAsiento* encontrado = busquedaBinaria(fila, numero, vuelo);
        if (encontrado) {
            encontrado->asiento.disponible = true;
        }
        else {
            // Maneja el caso donde no se encuentra el asiento
            //MessageBoxA(hwnd, "Asiento no encontrado.", "Error", MB_OK | MB_ICONERROR);
        }
    }


    void actualizar_listbox(HWND hwnd, int listboxID, int na) {
        HWND listbox = GetDlgItem(hwnd, listboxID);
        if (listbox != NULL) {
            SendMessage(listbox, LB_RESETCONTENT, 0, 0);

            NodoAsiento* temp = cabeza;
            while (temp != nullptr) {

                if (temp->asiento.numero == na) {
                    if (temp->asiento.disponible) {
                        char str[10];
                        sprintf_s(str, "%c%d", temp->asiento.fila, temp->asiento.numero);
                        int index = SendMessageA(listbox, LB_ADDSTRING, 0, (LPARAM)str);
                        SendMessage(listbox, LB_SETITEMDATA, index, (LPARAM)temp);
                    }
                }
                temp = temp->siguiente;
            }
        }
    }
    void guardar(const char* filename) {
        std::ofstream file(filename, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!file.is_open()) {
            std::cerr << "Error: no se pudo abrir el archivo de asientos." << std::endl;
            return;
        }
        NodoAsiento* temp = cabeza;
        while (temp != nullptr) {
            file.write(reinterpret_cast<char*>(&temp->asiento), sizeof(Asiento));
            temp = temp->siguiente;
        }
        file.close();
        std::cout << "Lista de asientos guardada exitosamente en el archivo '" << filename << "'." << std::endl;
    }
    void cargar(const char* filename) {
        std::ifstream file(filename, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error: no se pudo abrir el archivo de asientos." << std::endl;
            return;
        }
        Asiento asiento;
        while (file.read(reinterpret_cast<char*>(&asiento), sizeof(Asiento))) {
            agregarAlFinal(asiento);
        }
        file.close();
        std::cout << "Lista de asientos cargada exitosamente desde el archivo '" << filename << "'." << std::endl;
    }
    void intercambiar(NodoAsiento* a, NodoAsiento* b) {
        Asiento temp = a->asiento;
        a->asiento = b->asiento;
        b->asiento = temp;
    }

    NodoAsiento* particionar(NodoAsiento* bajo, NodoAsiento* alto) {
        int pivote = alto->asiento.numAvion; // Elegimos el pivote como el número de vuelo del asiento alto
        NodoAsiento* i = bajo->anterior;

        for (NodoAsiento* j = bajo; j != alto; j = j->siguiente) {
            if (j->asiento.numAvion <= pivote) {
                i = (i == nullptr) ? bajo : i->siguiente;
                intercambiar(i, j);
            }
        }
        i = (i == nullptr) ? bajo : i->siguiente;
        intercambiar(i, alto);
        return i;
    }

    void quicksort(NodoAsiento* bajo, NodoAsiento* alto) {
        if (alto != nullptr && bajo != alto && bajo != alto->siguiente) {
            NodoAsiento* pi = particionar(bajo, alto);
            quicksort(bajo, pi->anterior);
            quicksort(pi->siguiente, alto);
        }
    }

    void ordenarAsientosPorNumeroVuelo() {
        quicksort(cabeza, cola);
    }

    NodoAsiento* busquedaBinaria(char fila, int numero, int vuelo) {
        NodoAsiento* inicio = cabeza;
        NodoAsiento* fin = cola;

        while (inicio != fin && inicio->anterior != fin) {
            NodoAsiento* medio = inicio;
            int pasos = 0;
            for (NodoAsiento* temp = inicio; temp != fin; temp = temp->siguiente) {
                pasos++;
            }
            for (int i = 0; i < pasos / 2; ++i) {
                medio = medio->siguiente;
            }

            if (medio->asiento.numAvion == vuelo && medio->asiento.fila == fila && medio->asiento.numero == numero) {
                return medio;
            }
            if (medio->asiento.numAvion < vuelo || (medio->asiento.numAvion == vuelo && (medio->asiento.fila < fila || (medio->asiento.fila == fila && medio->asiento.numero < numero)))) {
                inicio = medio->siguiente;
            }
            else {
                fin = medio->anterior;
            }
        }
        if (inicio->asiento.numAvion == vuelo && inicio->asiento.fila == fila && inicio->asiento.numero == numero) {
            return inicio;
        }
        return nullptr;
    }

};

bool guardarListasEnArchivo(ListaDobleVuelo& listaVuelos, ListaDobleAsiento& listaAsientos, const char* filename) {
    ofstream archivo(filename);
    if (!archivo.is_open()) {
        cerr << "Error: no se pudo abrir el archivo '" << filename << "' para escritura." << endl;
        return false;
    }

    // Guardar lista de vuelos
    archivo << "=== Lista de Vuelos ===" << endl;
    NodoVuelo* nodoVuelo = listaVuelos.cabeza;
    while (nodoVuelo != nullptr) {
        archivo << "Origen: " << nodoVuelo->vuelo.Origen << endl;
        archivo << "Destino: " << nodoVuelo->vuelo.Destino << endl;
        archivo << "Fecha de Origen: " << nodoVuelo->vuelo.FechaOrigen << endl;
        archivo << "Hora de Origen: " << nodoVuelo->vuelo.HoraOrigen << endl;
        archivo << "Fecha de Destino: " << nodoVuelo->vuelo.FechaDestino << endl;
        archivo << "Hora de Destino: " << nodoVuelo->vuelo.HoraDestino << endl;
        archivo << "Número de Vuelo: " << nodoVuelo->vuelo.NumVuelo << endl;
        archivo << "Tipo de Avión: " << nodoVuelo->vuelo.TipoAvion << endl;
        archivo << "Número de Asientos Totales: " << nodoVuelo->vuelo.NumAsientosTot << endl;
        archivo << "Número de Asientos en Turista: " << nodoVuelo->vuelo.NumAsientoTu << endl;
        archivo << "Número de Asientos en Ejecutivo: " << nodoVuelo->vuelo.NumAsientoEj << endl;
        archivo << "Número de Menores: " << nodoVuelo->vuelo.Menores << endl;
        archivo << "Número de Mayores: " << nodoVuelo->vuelo.Mayores << endl;
        archivo << "Costo de Asiento en Turista: " << nodoVuelo->vuelo.CostoTu << endl;
        archivo << "Costo de Asiento en Ejecutivo: " << nodoVuelo->vuelo.CostoEj << endl;
        archivo << "Estatus: " << nodoVuelo->vuelo.Estatus << endl;
        archivo << "Fecha de Registro: " << nodoVuelo->vuelo.FechaRegistro << endl;
        archivo << "Asientos para Menores: " << nodoVuelo->vuelo.asientosMenores << endl;
        archivo << "Asientos para Mayores: " << nodoVuelo->vuelo.asientosMayores << endl;
        archivo << endl;
        nodoVuelo = nodoVuelo->siguiente;
    }

    archivo << "=== Lista de Asientos ===" << endl;
    // Guardar lista de asientos
    NodoAsiento* nodoAsiento = listaAsientos.cabeza;
    while (nodoAsiento != nullptr) {
        archivo << "Fila: " << nodoAsiento->asiento.fila << endl;
        archivo << "Número: " << nodoAsiento->asiento.numero << endl;
        archivo << "Disponible: " << (nodoAsiento->asiento.disponible ? "Sí" : "No") << endl;
        archivo << "Número de Avión: " << nodoAsiento->asiento.numAvion << endl;
        archivo << endl;
        nodoAsiento = nodoAsiento->siguiente;
    }

    archivo.close();
    cout << "Listas guardadas en el archivo '" << filename << "' exitosamente." << endl;
    return true;
}

