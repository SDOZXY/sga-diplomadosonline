// Pila.java
// Archivos Importados
import java.util.ArrayDeque;
import java.util.Deque;

// Pila generica LIFO: el ultimo elemento en entrar es el primero en salir.
// Usa un Deque (ArrayDeque) de java.util como estructura interna.
public class Pila<T> {

    // Atributo privado (encapsulamiento): nadie fuera de la clase toca el Deque
    private final Deque<T> items = new ArrayDeque<>();

    // Agrega el item en el tope de la pila
    public void apilar(T item) {
        items.push(item);
    }

    // Quita y devuelve el elemento del tope; lanza error si la pila esta vacia
    public T desapilar() {
        if (estaVacia()) {
            throw new IllegalStateException("La pila esta vacia.");
        }
        return items.pop();
    }

    // Devuelve el elemento del tope sin quitarlo; lanza error si la pila esta vacia
    public T verTope() {
        if (estaVacia()) {
            throw new IllegalStateException("La pila esta vacia.");
        }
        return items.peek();
    }

    // Devuelve true si la pila no posee elementos
    public boolean estaVacia() {
        return items.isEmpty();
    }

    // Devuelve cuantos elementos hay (equivale a len(Pila) en Python)
    public int tamano() {
        return items.size();
    }
}
