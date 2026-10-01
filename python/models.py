# models.py
# Archivos y modulos que estamos importando
from abc import ABC, abstractmethod
import math


# Clase base abstracta de todos los programas academicos
class ProgramaAcademico(ABC):
    # Nombre legible del programa
    nombre = "Programa"


    # Calcula el promedio de la lista de notas recibidas
    def calcular_promedio(self, notas):
        if not notas:
            return 0.0
        return sum(notas) / len(notas)


    # Metodo abstracto que obliga a las clases hijas a definir sus reglas de aprobacion
    @abstractmethod
    def evaluar_aprobacion(self, notas):
        pass


# Clase hija que representa un Curso (aprueba con promedio >= 10)
class Curso(ProgramaAcademico):
    # Sobrescribe el nombre del programa
    nombre = "Curso"


    # Evalua si el alumno aprueba el curso (promedio mayor o igual a 10)
    def evaluar_aprobacion(self, notas):
        return bool(notas) and self.calcular_promedio(notas) >= 10.0


# Clase hija que representa un Diplomado (aprueba con promedio >= 14)
class Diplomado(ProgramaAcademico):
    # Sobrescribe el nombre del programa
    nombre = "Diplomado"


    # Evalua si el alumno aprueba el diplomado (promedio mayor o igual a 14)
    def evaluar_aprobacion(self, notas):
        return bool(notas) and self.calcular_promedio(notas) >= 14.0


# Clase hija que representa un Bootcamp (exige notas y ninguna menor a 14)
class Bootcamp(ProgramaAcademico):
    # Sobrescribe el nombre del programa
    nombre = "Bootcamp"


    # Evalua si el alumno aprueba el bootcamp (todas las notas registradas >= 14)
    def evaluar_aprobacion(self, notas):
        return bool(notas) and all(n >= 14.0 for n in notas)


# Diccionario que relaciona la opcion elegida con la tupla (Nombre, Clase)
PROGRAMAS_DISPONIBLES = {
    "1": ("Curso", Curso),
    "2": ("Diplomado", Diplomado),
    "3": ("Bootcamp", Bootcamp),
}


# Devuelve el nombre del programa asociado al numero de opcion ingresado
def obtener_nombre_programa_por_opcion(opcion):
    datos = PROGRAMAS_DISPONIBLES.get(str(opcion).strip())
    return datos[0] if datos else None


# Fabrica que instancia el objeto del programa academico correspondiente a un nombre
def crear_programa(nombre):
    for nombre_valido, clase in PROGRAMAS_DISPONIBLES.values():
        if nombre_valido.lower() == str(nombre).strip().lower():
            return clase()
    raise ValueError(f"Programa no valido: '{nombre}'.")


# Clase base que contiene la informacion comun de cualquier persona en el sistema
class Persona:


    # Guarda cedula, nombre y correo de una persona.
    def __init__(self, cedula, nombre, correo):
        self.cedula = cedula
        self.nombre = nombre
        self.correo = correo



# Clase hija de Persona para representar a los estudiantes
class Alumno(Persona):


    # Inicializa los datos personales, el programa asignado y la lista de notas
    def __init__(self, cedula, nombre, correo, programa, notas=None):
        super().__init__(cedula, nombre, correo)
        self.programa = programa
        self.notas = notas if notas is not None else []


    # Valida que la nota este entre 0 y 20 y la agrega a la lista
    def agregar_nota(self, nota):
        if not 0 <= nota <= 20:
            raise ValueError("La nota debe estar entre 0 y 20.")
        self.notas.append(nota)


    # Elimina y retorna la ultima nota agregada
    def quitar_ultima_nota(self):
        if not self.notas:
            raise ValueError("El alumno no tiene notas para quitar.")
        return self.notas.pop()


    # Delega el calculo del promedio al programa en el que esta inscrito
    def promedio(self):
        return self.programa.calcular_promedio(self.notas)


    # Retorna el promedio formateado en texto truncado a un decimal
    def promedio_texto(self):
        return f"{math.floor(self.promedio() * 10) / 10:.1f}"


    # Aplica polimorfismo evaluando la aprobacion segun las reglas de su programa
    def esta_aprobado(self):
        return self.programa.evaluar_aprobacion(self.notas)


    # Genera la cadena formateada para guardar el alumno en alumnos.txt
    def a_linea(self):
        if self.notas:
            notas_txt = ", ".join(f"{n:g}" for n in self.notas)
        else:
            notas_txt = "Sin notas"
        return f"{self.cedula}, {self.nombre}, {self.correo}, {self.programa.nombre}, {notas_txt}"


# Clase hija de Persona para representar a los profesores
class Profesor(Persona):


    # Inicializa los datos de persona junto con la especialidad y materia
    def __init__(self, cedula, nombre, correo, especialidad, materia):
        super().__init__(cedula, nombre, correo)
        self.especialidad = especialidad
        self.materia = materia


    # Genera la cadena formateada para guardar el profesor en profesores.txt
    def a_linea(self):
        return f"{self.cedula}, {self.nombre}, {self.correo}, {self.especialidad}, {self.materia}"
