// main.cpp
// Archivos Importados
#include <cerrno>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include "gestor_sga.h"
#include "models.h"
#ifdef _WIN32
#include <windows.h>
#endif


namespace {

// Texto que se muestra cada vez que se ingresa un numero invalido
const char* const ERROR_NUMERICO = "Error: Ingrese un valor numérico válido";

// Excepcion propia para detectar si el usuario presiono Ctrl+Z o Ctrl+D y cerrar sin errores
class FinDeEntrada : public std::exception {
public:
    const char* what() const noexcept override { return "Fin de la entrada"; }
};

// Pide un texto por consola y le quita espacios vacios a los lados.
// Lanza FinDeEntrada si se interrumpe la lectura del teclado.
std::string pedirTexto(const std::string& mensaje) {
    std::cout << mensaje << std::flush;
    std::string linea;
    if (!std::getline(std::cin, linea)) {
        throw FinDeEntrada();
    }
    return recortar(linea);
}

// Pide una nota, valida que sea un numero real y la convierte a decimal
bool pedirNota(const std::string& mensaje, double& nota) {
    if (!convertirADouble(pedirTexto(mensaje), nota)) {
        std::cout << "  -> " << ERROR_NUMERICO << std::endl;
        return false;
    }
    return true;
}

// Imprime los programas academicos disponibles y retorna el elegido por el usuario
std::string pedirPrograma() {
    std::cout << "Programas disponibles:" << std::endl;
    const std::vector<OpcionPrograma>& tabla = programasDisponibles();
    for (std::size_t i = 0; i < tabla.size(); ++i) {
        std::cout << "  " << tabla[i].opcion << ". " << tabla[i].nombre << std::endl;
    }
    std::string nombre = obtenerNombreProgramaPorOpcion(pedirTexto("Elige el programa: "));
    if (nombre.empty()) {
        std::cout << "  -> Opcion de programa no valida." << std::endl;
    }
    return nombre;
}

// Opcion 1: solicita datos del alumno y lo registra a traves del gestor
void opcionRegistrarAlumno(GestorSGA& gestor) {
    std::string cedula = pedirTexto("Cedula: ");
    std::string nombre = pedirTexto("Nombre: ");
    std::string correo = pedirTexto("Correo: ");
    std::string programa = pedirPrograma();
    if (programa.empty()) {
        return;
    }
    gestor.registrarAlumno(cedula, nombre, correo, programa);
    std::cout << "  -> Alumno registrado y guardado en alumnos.txt." << std::endl;
}

// Opcion 2: solicita datos del profesor y lo registra a traves del gestor
void opcionRegistrarProfesor(GestorSGA& gestor) {
    std::string cedula = pedirTexto("Cedula: ");
    std::string nombre = pedirTexto("Nombre: ");
    std::string correo = pedirTexto("Correo: ");
    std::string especialidad = pedirTexto("Especialidad: ");
    std::string materia = pedirTexto("Materia: ");
    gestor.registrarProfesor(cedula, nombre, correo, especialidad, materia);
    std::cout << "  -> Profesor registrado y guardado en profesores.txt." << std::endl;
}

// Opcion 3: solicita la cedula y la nota a asignar a un alumno
void opcionRegistrarNota(GestorSGA& gestor) {
    std::string cedula = pedirTexto("Cedula del alumno: ");
    double nota = 0.0;
    if (!pedirNota("Nota (0-20): ", nota)) {
        return;
    }
    const Alumno& alumno = gestor.registrarNota(cedula, nota);
    std::size_t cantidad = alumno.getNotas().size();
    std::cout << "  -> Nota " << formatearNota(nota) << " registrada a " << alumno.getNombre()
              << " (" << cantidad << (cantidad == 1 ? " nota registrada)." : " notas registradas).")
              << std::endl;
}

// Opcion 4: borra la ultima nota agregada usando la pila (LIFO) del gestor
void opcionDeshacerNota(GestorSGA& gestor) {
    std::pair<std::string, double> resultado = gestor.deshacerNota();
    std::cout << "  -> Se elimino la nota " << formatearNota(resultado.second) << " de "
              << resultado.first << "." << std::endl;
}

// Opcion 5: procesa los alumnos aprobados en orden (FIFO) y crea los certificados
void opcionGenerarCertificados(GestorSGA& gestor) {
    std::size_t total = gestor.generarCertificados();
    std::cout << "  -> " << total << " graduando(s) exportados a certificados_pendientes.txt."
              << std::endl;
}

// Opcion 6: imprime la lista de profesores y alumnos registrados con su estatus actual
void opcionMostrarReporte(const GestorSGA& gestor) {
    std::vector<const Profesor*> profesores = gestor.getProfesoresOrdenados();
    std::vector<const Alumno*> alumnos = gestor.getAlumnosOrdenados();

    std::cout << "\n--- PROFESORES ACTIVOS ---" << std::endl;
    if (profesores.empty()) {
        std::cout << "(sin profesores registrados)" << std::endl;
    }
    for (std::size_t i = 0; i < profesores.size(); ++i) {
        const Profesor* p = profesores[i];
        std::cout << "[" << p->getCedula() << "] " << p->getNombre() << " | "
                  << p->getEspecialidad() << " | " << p->getMateria() << std::endl;
    }

    std::cout << "\n--- ALUMNOS ---" << std::endl;
    if (alumnos.empty()) {
        std::cout << "(sin alumnos registrados)" << std::endl;
    }
    for (std::size_t i = 0; i < alumnos.size(); ++i) {
        const Alumno* a = alumnos[i];
        std::string estatus = a->estaAprobado() ? "APROBADO" : "REPROBADO";
        std::string notas = a->getNotas().empty() ? "sin notas" : a->notasComoTexto();
        std::cout << "[" << a->getCedula() << "] " << a->getNombre() << " | "
                  << a->getPrograma().nombre() << " | Notas: " << notas
                  << " | Promedio: " << a->promedioTexto() << " | " << estatus << std::endl;
    }
}

// Imprime las opciones numeradas del menu
void mostrarMenu() {
    std::cout << "\n========== SGA - Diplomados Online ==========" << std::endl;
    std::cout << "1. Registrar Alumno" << std::endl;
    std::cout << "2. Registrar Profesor" << std::endl;
    std::cout << "3. Registrar Notas a un Alumno" << std::endl;
    std::cout << "4. Deshacer Ultimo Registro de Nota" << std::endl;
    std::cout << "5. Generar Cola de Certificados" << std::endl;
    std::cout << "6. Mostrar Reporte General" << std::endl;
    std::cout << "7. Salir" << std::endl;
}

// Lee la opcion del usuario y valida que sea un numero entero valido
bool leerOpcion(int& opcion) {
    const std::string texto = pedirTexto("Elige una opcion: ");
    char* fin = nullptr;
    errno = 0;
    long valor = std::strtol(texto.c_str(), &fin, 10);
    
    // Rechaza opciones vacias, letras o numeros extremadamente grandes
    if (texto.empty() || *fin != '\0' || errno == ERANGE || valor < -1000000 || valor > 1000000) {
        std::cout << "  -> " << ERROR_NUMERICO << std::endl;
        return false;
    }
    opcion = static_cast<int>(valor);
    return true;
}

// Ejecuta la funcion correspondiente segun la opcion elegida
bool ejecutarOpcion(int opcion, GestorSGA& gestor) {
    switch (opcion) {
        case 1: opcionRegistrarAlumno(gestor); return true;
        case 2: opcionRegistrarProfesor(gestor); return true;
        case 3: opcionRegistrarNota(gestor); return true;
        case 4: opcionDeshacerNota(gestor); return true;
        case 5: opcionGenerarCertificados(gestor); return true;
        case 6: opcionMostrarReporte(gestor); return true;
        case 7:
            gestor.guardarTodo();
            std::cout << "Datos guardados. Hasta luego." << std::endl;
            return false;
        default:
            std::cout << "  -> Opcion fuera de rango. Elige un numero del 1 al 7." << std::endl;
            return true;
    }
}

}  // namespace

// Funcion principal: punto de inicio de la ejecucion
int main() {
#ifdef _WIN32
    // Configura la consola de Windows para mostrar caracteres UTF-8 (tildes, etc.)
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    try {
        // Inicializa el gestor que carga la informacion guardada en los archivos .txt
        GestorSGA gestor;
        if (gestor.getLineasIgnoradas() > 0) {
            std::cout << "Aviso: se ignoraron " << gestor.getLineasIgnoradas()
                      << " linea(s) danada(s) al cargar los archivos." << std::endl;
        }

        // Bucle interactivo del programa
        bool continuar = true;
        while (continuar) {
            mostrarMenu();
            try {
                int opcion = 0;
                if (leerOpcion(opcion)) {
                    continuar = ejecutarOpcion(opcion, gestor);
                }
            } catch (const FinDeEntrada&) {
                // Captura el cierre inesperado de la consola y sale de forma limpia
                std::cout << "\nSaliendo... los datos ya estaban guardados." << std::endl;
                continuar = false;
            } catch (const std::exception& error) {
                // Captura cualquier otro error de validacion o de lectura de archivos
                std::cout << "  -> " << error.what() << std::endl;
            }
        }
    } catch (const std::exception& error) {
        std::cout << "  -> " << error.what() << std::endl;
        return 1;
    }
    return 0;
}
