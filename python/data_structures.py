# data_structures.py
# Archivos que estamos importando
from collections import deque


# Aca creamos la clase donde haremos funcionar la Pila(Stack)
class Pila:

    # Se Crea una pila vacia, usando una lista
    def __init__(self):
        self._items = []


    # Se agrega un item en el tope de la pila
    def apilar(self, item):
        self._items.append(item)


    # Se quita y devuelve el elemento del tope, y lanzara error si la pila esta vacia
    def desapilar(self):
        if self.esta_vacia():
            raise IndexError("La pila esta vacia.")
        return self._items.pop()


    # Devuelve el elemento del tope sin quitarlo, y lanzara error si la pila esta vacia
    def ver_tope(self):
        if self.esta_vacia():
            raise IndexError("La pila esta vacia.")
        return self._items[-1]


    # Devuelve True si la pila no posee elementos
    def esta_vacia(self):
        return len(self._items) == 0


    # Permite usar len(Pila) para conocer cuantos elementos hay
    def __len__(self):
        return len(self._items)


# Aca creamos la clase donde haremos funcionar la Cola(Queue)
class Cola:

    # Crea una cola vacia usando un deque
    def __init__(self):
        self._items = deque()


    # Agrega el item al final de la cola 
    def encolar(self, item):
        self._items.append(item)


    # Quita y devuelve el elemento del frente, lanzara error si esta vacio
    def desencolar(self):
        if self.esta_vacia():
            raise IndexError("La cola esta vacia.")
        return self._items.popleft()


    # Devuelve True si la cola no tiene elementos
    def esta_vacia(self):
        return len(self._items) == 0


    # Permite usar len(Cola) para conocer cuantos elementos hay
    def __len__(self):
        """Permite usar len(cola) para conocer cuantos elementos hay."""
        return len(self._items)
