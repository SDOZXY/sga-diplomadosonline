// Cola.java
// Archivos Importados
import java.util.ArrayDeque;
import java.util.Deque;


// Usa un Deque (ArrayDeque) de java.util como estructura interna.
public class Cola<T> {

    // Atributo privado (encapsulamiento)
    private final Deque<T> items = new ArrayDeque<>();

    // Agrega el item al final de la cola
    public void encolar(T item) {
        items.addLast(item);
    }

    // Quita y devuelve el elemento del frente; lanza error si la cola esta vacia
    public T desencolar() {
        if (estaVacia()) {
            throw new IllegalStateException("La cola esta vacia.");
        }
        return items.pollFirst();
    }

    // Devuelve true si la cola no tiene elementos
    public boolean estaVacia() {
        return items.isEmpty();
    }

    // Devuelve cuantos elementos hay
    public int tamano() {
        return items.size();
    }
}
