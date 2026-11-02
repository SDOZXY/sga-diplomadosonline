// data_structures.h
// archivos importados
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H
#include <cstddef>
#include <stdexcept>
#include <utility>


// Pila (LIFO: El ultimo en entrar es el primero en salir)
template <typename T>
class Pila {
private:
    // Estructura de un nodo individual en la pila
    struct Nodo {
        T dato;          // Informacion guardada en el nodo
        Nodo* siguiente; // Puntero al nodo ubicado debajo en la pila

        Nodo(const T& d, Nodo* s) : dato(d), siguiente(s) {}
    };

    Nodo* tope_;           // Apunta al nodo superior actual de la pila
    std::size_t cantidad_; // Lleva la cuenta de cuántos elementos hay guardados

public:
    // Constructor: Inicializa la pila vacia (sin elementos)
    Pila() : tope_(nullptr), cantidad_(0) {}

    // Destructor: Vacia y libera todos los nodos de la memoria dinamica
    ~Pila() { vaciar(); }

    // Deshabilita la copia para prevenir accesos o liberaciones dobles de memoria
    Pila(const Pila&) = delete;
    Pila& operator=(const Pila&) = delete;

    // Coloca un nuevo elemento en la parte superior (tope) de la pila
    void apilar(const T& item) {
        tope_ = new Nodo(item, tope_);
        ++cantidad_;
    }

    // Remueve y retorna el elemento del tope; lanza error si la pila esta vacia
    T desapilar() {
        if (estaVacia()) {
            throw std::out_of_range("La pila esta vacia.");
        }
        Nodo* nodo = tope_;
        tope_ = nodo->siguiente;
        --cantidad_;
        T dato(std::move(nodo->dato));
        delete nodo; // Libera el nodo sacado
        return dato;
    }

    // Consulta el valor en el tope sin quitarlo de la pila
    const T& verTope() const {
        if (estaVacia()) {
            throw std::out_of_range("La pila esta vacia.");
        }
        return tope_->dato;
    }

    // Verifica si la pila no tiene elementos asignados
    bool estaVacia() const { return tope_ == nullptr; }

    // Retorna la cantidad total de elementos apilados
    std::size_t tamano() const { return cantidad_; }

    // Remueve todos los elementos acumulados liberando memoria nodo a nodo
    void vaciar() {
        while (!estaVacia()) {
            desapilar();
        }
    }
};

// Cola (FIFO: El primero en entrar es el primero en salir)
template <typename T>
class Cola {
private:
    // Estructura de un nodo individual en la cola
    struct Nodo {
        T dato;          // Informacion guardada en el nodo
        Nodo* siguiente; // Puntero al siguiente nodo que le sigue en la fila

        explicit Nodo(const T& d) : dato(d), siguiente(nullptr) {}
    };

    Nodo* frente_;         // Apunta al primer elemento listo para salir
    Nodo* final_;          // Apunta al ultimo elemento ingresado
    std::size_t cantidad_; // Lleva la cuenta de cuántos elementos hay guardados

public:
    // Constructor: Inicializa la cola vacia
    Cola() : frente_(nullptr), final_(nullptr), cantidad_(0) {}

    // Destructor: Libera de memoria todos los nodos remanentes
    ~Cola() { vaciar(); }

    // Deshabilita la copia para evitar problemas de punteros colgados
    Cola(const Cola&) = delete;
    Cola& operator=(const Cola&) = delete;

    // Agrega un nuevo elemento al final de la cola
    void encolar(const T& item) {
        Nodo* nuevo = new Nodo(item);
        if (final_ == nullptr) {
            frente_ = nuevo; // Si estaba vacia, el nuevo nodo tambien es el frente
        } else {
            final_->siguiente = nuevo;
        }
        final_ = nuevo;
        ++cantidad_;
    }

    // Extrae y retorna el elemento ubicado al frente de la cola
    T desencolar() {
        if (estaVacia()) {
            throw std::out_of_range("La cola esta vacia.");
        }
        Nodo* nodo = frente_;
        frente_ = nodo->siguiente;
        if (frente_ == nullptr) {
            final_ = nullptr; // Si no quedan elementos, resetea el puntero final
        }
        --cantidad_;
        T dato(std::move(nodo->dato));
        delete nodo; // Libera el nodo retirado
        return dato;
    }

    // Comprueba si la cola carece de elementos
    bool estaVacia() const { return frente_ == nullptr; }

    // Devuelve el numero total de elementos retenidos en la cola
    std::size_t tamano() const { return cantidad_; }

    // Elimina secuencialmente todos los elementos de la cola
    void vaciar() {
        while (!estaVacia()) {
            desencolar();
        }
    }
};

#endif  // DATA_STRUCTURES_H
