
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

struct Boleto {
    char NombrePasajero[100];
    char ApellidoP[100];
    char ApellidoM[100];
    char origen[100];
    char destino[100];
    char fechaSalida[100];
    char horaSalida[100];
    int numVuelo;
    int costoBoleto;
    char tipoBoleto[100];
    char boletoGenerado[100];
    char metodoPago[100];
    char estatus[100] = "Activo";
    char Aletra;
    int Anumero;
};

struct NodoBoleto {
    Boleto boleto;
    NodoBoleto* siguiente;
    NodoBoleto* anterior;
    // Constructor
    NodoBoleto(const Boleto& b) : boleto(b), siguiente(nullptr), anterior(nullptr) {}
};


class ListaDobleBoleto {
public:

 NodoBoleto* cabeza;
    NodoBoleto* cola;
    ListaDobleBoleto() : cabeza(nullptr), cola(nullptr) {}

    // Función para agregar un boleto a la lista
    void agregar(const Boleto& b) {
        NodoBoleto* nuevo = new NodoBoleto(b);
        if (!cabeza) {
            cabeza = cola = nuevo;
        }
        else {
            cola->siguiente = nuevo;
            nuevo->anterior = cola;
            cola = nuevo;
        }
    }

    // Función para guardar la lista en un archivo binario
    void guardar(const char* filename) {
        std::ofstream archivo(filename, std::ios::binary | std::ios::trunc);
        if (!archivo) {
            std::cerr << "Error: no se pudo abrir el archivo para guardar." << std::endl;
            return;
        }
        NodoBoleto* actual = cabeza;
        while (actual) {
            archivo.write(reinterpret_cast<char*>(&actual->boleto), sizeof(Boleto));
            actual = actual->siguiente;
        }
        archivo.close();
        std::cout << "Lista de boletos guardada exitosamente en " << filename << std::endl;
    }

    // Función para cargar la lista desde un archivo binario
    void cargar(const char* filename) {
        std::ifstream archivo(filename, std::ios::binary);
        if (!archivo) {
            std::cerr << "Error: no se pudo abrir el archivo para cargar." << std::endl;
            return;
        }
        Boleto b;
        while (archivo.read(reinterpret_cast<char*>(&b), sizeof(Boleto))) {
            agregar(b);
        }
        archivo.close();
        std::cout << "Lista de boletos cargada exitosamente desde " << filename << std::endl;
    }

    void intercambiar(NodoBoleto* a, NodoBoleto* b) {
        Boleto temp = a->boleto;
        a->boleto = b->boleto;
        b->boleto = temp;
    }

    NodoBoleto* avanzar(NodoBoleto* nodo, int pasos) {
        while (nodo != nullptr && pasos > 0) {
            nodo = nodo->siguiente;
            pasos--;
        }
        return nodo;
    }

    NodoBoleto* retroceder(NodoBoleto* nodo, int pasos) {
        while (nodo != nullptr && pasos > 0) {
            nodo = nodo->anterior;
            pasos--;
        }
        return nodo;
    }

    void heapify(NodoBoleto* inicio, int n, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        NodoBoleto* largestNode = avanzar(inicio, largest);
        NodoBoleto* leftNode = avanzar(inicio, left);
        NodoBoleto* rightNode = avanzar(inicio, right);

        if (left < n && (leftNode->boleto.numVuelo > largestNode->boleto.numVuelo ||
            (leftNode->boleto.numVuelo == largestNode->boleto.numVuelo && strcmp(leftNode->boleto.NombrePasajero, largestNode->boleto.NombrePasajero) > 0) ||
            (leftNode->boleto.numVuelo == largestNode->boleto.numVuelo && strcmp(leftNode->boleto.NombrePasajero, largestNode->boleto.NombrePasajero) == 0 && strcmp(leftNode->boleto.ApellidoP, largestNode->boleto.ApellidoP) > 0) ||
            (leftNode->boleto.numVuelo == largestNode->boleto.numVuelo && strcmp(leftNode->boleto.NombrePasajero, largestNode->boleto.NombrePasajero) == 0 && strcmp(leftNode->boleto.ApellidoP, largestNode->boleto.ApellidoP) == 0 && strcmp(leftNode->boleto.ApellidoM, largestNode->boleto.ApellidoM) > 0))) {
            largest = left;
        }

        largestNode = avanzar(inicio, largest);

        if (right < n && (rightNode->boleto.numVuelo > largestNode->boleto.numVuelo ||
            (rightNode->boleto.numVuelo == largestNode->boleto.numVuelo && strcmp(rightNode->boleto.NombrePasajero, largestNode->boleto.NombrePasajero) > 0) ||
            (rightNode->boleto.numVuelo == largestNode->boleto.numVuelo && strcmp(rightNode->boleto.NombrePasajero, largestNode->boleto.NombrePasajero) == 0 && strcmp(rightNode->boleto.ApellidoP, largestNode->boleto.ApellidoP) > 0) ||
            (rightNode->boleto.numVuelo == largestNode->boleto.numVuelo && strcmp(rightNode->boleto.NombrePasajero, largestNode->boleto.NombrePasajero) == 0 && strcmp(rightNode->boleto.ApellidoP, largestNode->boleto.ApellidoP) == 0 && strcmp(rightNode->boleto.ApellidoM, largestNode->boleto.ApellidoM) > 0))) {
            largest = right;
        }

        if (largest != i) {
            NodoBoleto* iNode = avanzar(inicio, i);
            largestNode = avanzar(inicio, largest);
            intercambiar(iNode, largestNode);
            heapify(inicio, n, largest);
        }
    }

    void heapSort() {
        int n = 0;
        NodoBoleto* temp = cabeza;
        while (temp != nullptr) {
            n++;
            temp = temp->siguiente;
        }

        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(cabeza, n, i);
        }

        for (int i = n - 1; i > 0; i--) {
            intercambiar(cabeza, avanzar(cabeza, i));
            heapify(cabeza, i, 0);
        }
    }
    void ordenarBoletos() {
        heapSort();
    }

    NodoBoleto* busquedaBinaria(int numVuelo, const char* nombre, const char* apellidoP, const char* apellidoM) {
        if (cabeza == nullptr || cola == nullptr) {
            return nullptr; // La lista está vacía
        }

        NodoBoleto* inicio = cabeza;
        NodoBoleto* fin = cola;

        while (inicio != fin && inicio != nullptr && fin != nullptr && inicio->anterior != fin) {
            NodoBoleto* medio = inicio;
            int pasos = 0;

            for (NodoBoleto* temp = inicio; temp != fin->siguiente && temp != nullptr; temp = temp->siguiente) {
                pasos++;
            }

            for (int i = 0; i < pasos / 2 && medio != nullptr; ++i) {
                medio = medio->siguiente;
            }

            if (medio == nullptr) {
                return nullptr; // Si 'medio' es nulo, salimos de la función
            }

            if (medio->boleto.numVuelo == numVuelo &&
                strcmp(medio->boleto.NombrePasajero, nombre) == 0 &&
                strcmp(medio->boleto.ApellidoP, apellidoP) == 0 &&
                strcmp(medio->boleto.ApellidoM, apellidoM) == 0) {
                return medio;
            }

            if (medio->boleto.numVuelo < numVuelo ||
                (medio->boleto.numVuelo == numVuelo && strcmp(medio->boleto.NombrePasajero, nombre) < 0) ||
                (medio->boleto.numVuelo == numVuelo && strcmp(medio->boleto.NombrePasajero, nombre) == 0 && strcmp(medio->boleto.ApellidoP, apellidoP) < 0) ||
                (medio->boleto.numVuelo == numVuelo && strcmp(medio->boleto.NombrePasajero, nombre) == 0 && strcmp(medio->boleto.ApellidoP, apellidoP) == 0 && strcmp(medio->boleto.ApellidoM, apellidoM) < 0)) {
                inicio = medio->siguiente;
            }
            else {
                fin = medio->anterior;
            }
        }

        if (inicio != nullptr &&
            inicio->boleto.numVuelo == numVuelo &&
            strcmp(inicio->boleto.NombrePasajero, nombre) == 0 &&
            strcmp(inicio->boleto.ApellidoP, apellidoP) == 0 &&
            strcmp(inicio->boleto.ApellidoM, apellidoM) == 0) {
            return inicio;
        }

        return nullptr;
    }

    bool verificarBoletoExistente(int numVuelo, const char* nombre, const char* apellidoP, const char* apellidoM) {
        return busquedaBinaria(numVuelo, nombre, apellidoP, apellidoM) != nullptr;
    }


    void agregarBoleto(const Boleto& b) {
        if (!verificarBoletoExistente(b.numVuelo, b.NombrePasajero, b.ApellidoP, b.ApellidoM)) {
            agregar(b);
            ordenarBoletos(); // Aseguramos que la lista esté ordenada después de agregar un nuevo boleto
        }
    }

    NodoBoleto* buscarBinario(int numVuelo) {
        std::vector<Boleto> boletos;
        NodoBoleto* temp = cabeza;
        while (temp != nullptr) {
            boletos.push_back(temp->boleto);
            temp = temp->siguiente;
        }

        int izquierda = 0;
        int derecha = boletos.size() - 1;
        while (izquierda <= derecha) {
            int medio = izquierda + (derecha - izquierda) / 2;
            if (boletos[medio].numVuelo == numVuelo) {
                // Encontrado
                temp = cabeza;
                while (temp != nullptr) {
                    if (temp->boleto.numVuelo == numVuelo) {
                        return temp;
                    }
                    temp = temp->siguiente;
                }
            }
            if (boletos[medio].numVuelo < numVuelo) {
                izquierda = medio + 1;
            }
            else {
                derecha = medio - 1;
            }
        }

        return nullptr;
    }
};