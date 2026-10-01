# main.py
# Archivos y modulos que estamos importando
from gestor_sga import GestorSGA
from models import PROGRAMAS_DISPONIBLES, obtener_nombre_programa_por_opcion


# Pide un texto por teclado y elimina espacios al inicio y final
def pedir_texto(mensaje):
    return input(mensaje).strip()


# Pide un numero permitiendo coma o punto decimal y devuelve None si el dato no es valido
def pedir_nota(mensaje):
    try:
        return float(input(mensaje).replace(",", "."))
    except ValueError:
        print("  -> Debes ingresar un numero valido (ejemplo: 15 o 14.5).")
        return None


# Muestra los programas disponibles y devuelve el nombre elegido o None si es invalido
def pedir_programa():
    print("Programas disponibles:")
    for opcion, (nombre, _clase) in PROGRAMAS_DISPONIBLES.items():
        print(f"  {opcion}. {nombre}")
    nombre = obtener_nombre_programa_por_opcion(input("Elige el programa: "))
    if nombre is None:
        print("  -> Opcion de programa no valida.")
    return nombre


# Opcion 1: Pide los datos del alumno y se los entrega al gestor para registrarlo
def opcion_registrar_alumno(gestor):
    cedula = pedir_texto("Cedula: ")
    nombre = pedir_texto("Nombre: ")
    correo = pedir_texto("Correo: ")
    programa = pedir_programa()
    if programa is None:
        return
    gestor.registrar_alumno(cedula, nombre, correo, programa)
    print("  -> Alumno registrado y guardado en alumnos.txt.")


# Opcion 2: Pide los datos del profesor y se los entrega al gestor para registrarlo
def opcion_registrar_profesor(gestor):
    cedula = pedir_texto("Cedula: ")
    nombre = pedir_texto("Nombre: ")
    correo = pedir_texto("Correo: ")
    especialidad = pedir_texto("Especialidad: ")
    materia = pedir_texto("Materia: ")
    gestor.registrar_profesor(cedula, nombre, correo, especialidad, materia)
    print("  -> Profesor registrado y guardado en profesores.txt.")


# Opcion 3: Pide cedula y nota del alumno para registrarla en el sistema
def opcion_registrar_nota(gestor):
    cedula = pedir_texto("Cedula del alumno: ")
    nota = pedir_nota("Nota (0-20): ")
    if nota is None:
        return
    alumno = gestor.registrar_nota(cedula, nota)
    cant_notas = len(alumno.notas)
    texto_notas = "nota registrada" if cant_notas == 1 else "notas registradas"
    print(f"  -> Nota {nota:g} registrada a {alumno.nombre} "
          f"({cant_notas} {texto_notas}).")


# Opcion 4: Deshace la ultima nota registrada utilizando la pila (LIFO)
def opcion_deshacer_nota(gestor):
    alumno, nota = gestor.deshacer_nota()
    print(f"  -> Se elimino la nota {nota:g} de {alumno.nombre}.")


# Opcion 5: Genera la cola de alumnos aprobados y exporta el reporte de certificados
def opcion_generar_certificados(gestor):
    total = gestor.generar_certificados()
    print(f"  -> {total} graduando(s) exportados a certificados_pendientes.txt.")


# Opcion 6: Imprime en consola la lista de profesores y alumnos registrados
def opcion_mostrar_reporte(gestor):
    profesores, alumnos = gestor.obtener_reporte()
    print("\n--- PROFESORES ACTIVOS ---")
    if not profesores:
        print("(sin profesores registrados)")
    for p in profesores:
        print(f"[{p.cedula}] {p.nombre} | {p.especialidad} | {p.materia}")
    print("\n--- ALUMNOS ---")
    if not alumnos:
        print("(sin alumnos registrados)")
    for a in alumnos:
        estatus = "APROBADO" if a.esta_aprobado() else "REPROBADO"
        notas = ", ".join(f"{n:g}" for n in a.notas) or "sin notas"
        print(f"[{a.cedula}] {a.nombre} | {a.programa.nombre} | Notas: {notas} | "
              f"Promedio: {a.promedio_texto()} | {estatus}")


# Imprime las opciones del menu principal en consola
def mostrar_menu():
    print("\n========== SGA - Diplomados Online ==========")
    print("1. Registrar Alumno")
    print("2. Registrar Profesor")
    print("3. Registrar Notas a un Alumno")
    print("4. Deshacer Ultimo Registro de Nota")
    print("5. Generar Cola de Certificados")
    print("6. Mostrar Reporte General")
    print("7. Salir")


# Lee la opcion elegida por el usuario y valida que sea un entero
def leer_opcion():
    try:
        return int(input("Elige una opcion: "))
    except ValueError:
        print("  -> Por favor ingresa un numero del 1 al 7.")
        return None


# Funcion principal que ejecuta el bucle e interactua con el usuario
def main():
    gestor = GestorSGA()
    if gestor.lineas_ignoradas:
        print(f"Aviso: se ignoraron {gestor.lineas_ignoradas} linea(s) danada(s) al cargar los archivos.")


    # Diccionario que mapea la opcion del menu con su funcion correspondiente
    acciones = {
        1: opcion_registrar_alumno,
        2: opcion_registrar_profesor,
        3: opcion_registrar_nota,
        4: opcion_deshacer_nota,
        5: opcion_generar_certificados,
        6: opcion_mostrar_reporte,
    }

    while True:
        mostrar_menu()
        try:
            opcion = leer_opcion()
            if opcion is None:
                continue
            if opcion == 7:
                gestor.guardar_todo()
                print("Datos guardados. Hasta luego.")
                break
            if opcion not in acciones:
                print("  -> Opcion fuera de rango. Elige un numero del 1 al 7.")
                continue
            acciones[opcion](gestor)
        except ValueError as error:
            # Captura y muestra errores de validacion lanzados por el gestor
            print(f"  -> {error}")
        except (EOFError, KeyboardInterrupt):
            print("\nSaliendo... los datos ya estaban guardados.")
            break


# Punto de entrada para ejecutar el programa
if __name__ == "__main__":
    main()
