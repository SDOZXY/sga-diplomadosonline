# gestor_sga.py
# Archivos que estamos importando
import os
from data_structures import Cola, Pila
from models import Alumno, Profesor, ProgramaAcademico, crear_programa


# Aca estan los Archivos TXT que se guardaran junto a este archivo
CARPETA_DATOS = os.path.dirname(os.path.abspath(__file__))
ARCH_ALUMNOS = os.path.join(CARPETA_DATOS, "alumnos.txt")
ARCH_PROFESORES = os.path.join(CARPETA_DATOS, "profesores.txt")
ARCH_CERTIFICADOS = os.path.join(CARPETA_DATOS, "certificados_pendientes.txt")


# Esta clase es la que se encarga de sincronizar y guardar los datos que ingresamos en los archivos TXT
class GestorSGA:
    def __init__(self):
        self.alumnos = {}
        self.profesores = {}
        self.pila_deshacer = Pila()  # Aca se coloca la Pila, la cual guarda la cedula del alumno de cada nota agregada
        self.cargar_datos()


    # Este metodo estatico lo usamos para quitar los espacios y comas al momento de ingresar datos y evita que se guarden campos vacios
    @staticmethod
    def _limpiar(texto, campo):
        limpio = " ".join(str(texto).replace(",", " ").split())
        if not limpio:
            raise ValueError(f"El campo '{campo}' no puede estar vacio.")
        return limpio

    # Este metodo estatico tiene de funcion, el validar que en el campo de correo, 
    # deben estar presentes el simbolo @ y despues de dicho simbolo haya un .
    @staticmethod
    def _validar_correo(correo):
        if "@" not in correo or "." not in correo.split("@")[-1]:
            raise ValueError("El correo no parece valido (ejemplo: ana@email.com).") # Aca dejamos un ejemplo, de la manera correcta en la que debe ingresarse el correo


    # Esta declaracion sirve para mostrar el alumno que tenga dicha cedula que se busca, si no existe lanzara error
    def _buscar_alumno(self, cedula):
        alumno = self.alumnos.get(self._limpiar(cedula, "Cedula"))
        if alumno is None:
            raise ValueError("No existe un alumno con esa cedula.")
        return alumno


    # Esta declaracion es la encargada de reescribir el archivo alumnos.txt (se encarga de la persistencia inmediata)
    def guardar_alumnos(self):
        with open(ARCH_ALUMNOS, "w", encoding="utf-8") as f:
            for alumno in self.alumnos.values():
                f.write(alumno.a_linea() + "\n")


    # Esta declaracion es la encargada de reescribir el archivo profesores.txt (se encarga de la persistencia inmediata)
    def guardar_profesores(self):
        with open(ARCH_PROFESORES, "w", encoding="utf-8") as f:
            for profesor in self.profesores.values():
                f.write(profesor.a_linea() + "\n")

    # Esta declaracion se encarga de leer los archivos .txt al iniciar el sistema, 
    # Las lineas dañadas se ignoran en lugar de romper el sistema (dichas lineas ignoradas son contadas)
    def cargar_datos(self):
        self.lineas_ignoradas = 0
        if os.path.exists(ARCH_ALUMNOS):
            with open(ARCH_ALUMNOS, encoding="utf-8") as f:
                for linea in f:
                    if linea.strip():
                        self._cargar_linea_alumno(linea)
        if os.path.exists(ARCH_PROFESORES):
            with open(ARCH_PROFESORES, encoding="utf-8") as f:
                for linea in f:
                    if linea.strip():
                        self._cargar_linea_profesor(linea)


    # Esta declaracion convierte en un alumno, una linea del archivo alumno.txt al iniciar el programa
    def _cargar_linea_alumno(self, linea):
        try:
            p = [x.strip() for x in linea.split(",")]
            if len(p) < 4:
                raise ValueError
            cedula, nombre, correo, nombre_programa = p[0], p[1], p[2], p[3]
            programa = crear_programa(nombre_programa)
            notas= []
            if len(p) > 4:
                resto_notas = p[4:]
                for n in resto_notas:
                    if n and n != "Sin notas" and n != "-":
                        notas.append(float(n))
            self.alumnos[cedula] = Alumno(cedula, nombre, correo, programa, notas)
        except ValueError:
            self.lineas_ignoradas += 1


    # Esta declaracion convierte en un profesor, una linea del archivo profesores.txt al iniciar el programa e ignora las lineas vacias
    def _cargar_linea_profesor(self, linea):
        p = [x.strip() for x in linea.split(",")]
        if len(p) == 5:
            self.profesores[p[0]] = Profesor(*p)
        else:
            self.lineas_ignoradas += 1


    # Esta declaracion se encarga de crear la opcion 1, la cual seria validar, registrar el Alumno y almacenarlo en alumnos.txt
    def registrar_alumno(self, cedula, nombre, correo, nombre_programa):
        cedula = self._limpiar(cedula, "Cedula")
        nombre = self._limpiar(nombre, "Nombre")
        correo = self._limpiar(correo, "Correo")
        self._validar_correo(correo)
        if cedula in self.alumnos:
            raise ValueError("Ya existe un alumno con esa cedula.")
        programa: ProgramaAcademico = crear_programa(nombre_programa)
        self.alumnos[cedula] = Alumno(cedula, nombre, correo, programa)
        self.guardar_alumnos()


    # Esta declaracion se encarga de crear la opcion 2, la cual seria validar, registrar al profesor y almacenarlo en profesores.txt
    def registrar_profesor(self, cedula, nombre, correo, especialidad, materia):
        cedula = self._limpiar(cedula, "Cedula")
        nombre = self._limpiar(nombre, "Nombre")
        correo = self._limpiar(correo, "Correo")
        self._validar_correo(correo)
        especialidad = self._limpiar(especialidad, "Especialidad")
        materia = self._limpiar(materia, "Materia")
        if cedula in self.profesores:
            raise ValueError("Ya existe un profesor con esa cedula.")
        self.profesores[cedula] = Profesor(cedula, nombre, correo, especialidad, materia)
        self.guardar_profesores()

    # Esta declaracion se encarga de crear la opcion 3, la cual seria agregar la nota del alumno, y apila la accion en el LIFO(PILA),
    # y persiste en alumnos.txt
    def registrar_nota(self, cedula, nota):
        alumno = self._buscar_alumno(cedula)
        alumno.agregar_nota(nota)            # Se coloco un rango de 0 hasta 20, siendo 20 el numero max valido como nota
        self.pila_deshacer.apilar(alumno.cedula)
        self.guardar_alumnos()
        return alumno

    # Esta declaracion se encarga de crear la opcion 4, la cual seria desapila la ultima accion, 
    # la cual seria quitar la ultima nota registrada del alumno en el momento
    def deshacer_nota(self):
        if self.pila_deshacer.esta_vacia():
            raise ValueError("No hay notas para deshacer.")
        cedula = self.pila_deshacer.desapilar()
        alumno = self.alumnos[cedula]
        nota = alumno.quitar_ultima_nota()
        self.guardar_alumnos()
        return alumno, nota

    # Esta declaracion se encarga de crear la opcion 5, la cual seria evaluar a todos los alumnos, 
    # coloca en cola a los aprobados(FIFO) y los exporta como un archivo certificados_pendientes.txt
    def generar_certificados(self):
        cola = Cola()
        for alumno in self.alumnos.values():
            if alumno.esta_aprobado():
                cola.encolar(alumno)
        total = len(cola)
        with open(ARCH_CERTIFICADOS, "w", encoding="utf-8") as f:
            f.write("=== REPORTE DE CERTIFICADOS PENDIENTES ===\n")
            f.write(f"Total de graduandos en cola: {total}\n\n")
            posicion = 1
            while not cola.esta_vacia():
                alumno = cola.desencolar()   # sale en el mismo orden en que entro
                f.write(f"{posicion}. [{alumno.cedula}] {alumno.nombre}\n")
                f.write(f"   - Programa: {alumno.programa.nombre}\n")
                f.write(f"   - Promedio Final: {alumno.promedio_texto()}\n")
                f.write("   - Estatus: APROBADO\n\n")
                posicion += 1
        return total


    # Esta declaracion se encarga de crear la opcion 6, la cual seria devolver o mostrar a los profesores y alumnos(ordenados por cedula)
    def obtener_reporte(self):
        profesores = sorted(self.profesores.values(), key=lambda p: p.cedula)
        alumnos = sorted(self.alumnos.values(), key=lambda a: a.cedula)
        return profesores, alumnos


    # Esta declaracion se encarga de crear la opcion 7, cuya unica utilidad es Salir o Finalizar el sistema de manera segura
    def guardar_todo(self):
        """Opcion 7: guardado final de seguridad de ambos archivos antes de salir."""
        self.guardar_alumnos()
        self.guardar_profesores()
