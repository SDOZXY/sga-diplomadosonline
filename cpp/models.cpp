// models.cpp
// Archivos importados
#include "models.h"
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <stdexcept>


// Quita los espacios vacios y saltos de linea al inicio y al final de un texto
std::string recortar(const std::string& texto) {
    const std::string espacios = " \t\r\n";
    std::string::size_type inicio = texto.find_first_not_of(espacios);
    if (inicio == std::string::npos) {
        return "";
    }
    std::string::size_type fin = texto.find_last_not_of(espacios);
    return texto.substr(inicio, fin - inicio + 1);
}

// Convierte un texto en un numero decimal (acepta coma o punto)
// Devuelve false si el texto no es un numero valido
bool convertirADouble(const std::string& texto, double& resultado) {
    std::string limpio = recortar(texto);
    
    // Cambia comas por puntos para que la conversion no falle
    for (std::string::size_type i = 0; i < limpio.size(); ++i) {
        if (limpio[i] == ',') {
            limpio[i] = '.';
        }
    }
    
    // Rechaza textos vacios o valores en formato hexadecimal
    if (limpio.empty() || limpio.find_first_of("xX") != std::string::npos) {
        return false;
    }
    
    char* fin = nullptr;
    double valor = std::strtod(limpio.c_str(), &fin);
    
    // Valida que se haya procesado todo el texto y que sea un numero real
    if (*fin != '\0' || !std::isfinite(valor)) {
        return false;
    }
    resultado = valor;
    return true;
}

// Convierte un numero de nota a texto para mostrarlo formateado
std::string formatearNota(double nota) {
    std::ostringstream salida;
    salida << nota;
    return salida.str();
}

namespace {

// Pasa todo un texto a minusculas para comparar palabras sin importar Mayusculas/Minusculas
std::string aMinusculas(std::string texto) {
    for (std::string::size_type i = 0; i < texto.size(); ++i) {
        texto[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(texto[i])));
    }
    return texto;
}

// Funciones auxiliares para crear cada tipo de programa en memoria dinamica
ProgramaAcademico* crearCurso() { return new Curso(); }
ProgramaAcademico* crearDiplomado() { return new Diplomado(); }
ProgramaAcademico* crearBootcamp() { return new Bootcamp(); }

}  // namespace


// Calcula el promedio simple de una lista de notas (devuelve 0.0 si no hay notas)
double ProgramaAcademico::calcularPromedio(const std::vector<double>& notas) const {
    if (notas.empty()) {
        return 0.0;
    }
    double suma = 0.0;
    for (std::size_t i = 0; i < notas.size(); ++i) {
        suma += notas[i];
    }
    return suma / static_cast<double>(notas.size());
}

// Nombre de la clase Curso
std::string Curso::nombre() const { return "Curso"; }

// Regla de Curso: aprueba si tiene notas y el promedio es igual o mayor a 10
bool Curso::evaluarAprobacion(const std::vector<double>& notas) const {
    return !notas.empty() && calcularPromedio(notas) >= 10.0;
}

// Nombre de la clase Diplomado
std::string Diplomado::nombre() const { return "Diplomado"; }

// Regla de Diplomado: aprueba si tiene notas y el promedio es igual o mayor a 14
bool Diplomado::evaluarAprobacion(const std::vector<double>& notas) const {
    return !notas.empty() && calcularPromedio(notas) >= 14.0;
}

// Nombre de la clase Bootcamp
std::string Bootcamp::nombre() const { return "Bootcamp"; }

// Regla de Bootcamp: requiere 3 notas y NINGUNA puede ser menor a 14
bool Bootcamp::evaluarAprobacion(const std::vector<double>& notas) const {
    if (notas.empty()) {
        return false;
    }
    for (std::size_t i = 0; i < notas.size(); ++i) {
        if (notas[i] < 14.0) {
            return false;
        }
    }
    return true;
}

// Tabla con las opciones del menu para registrar o crear programas
const std::vector<OpcionPrograma>& programasDisponibles() {
    static const std::vector<OpcionPrograma> tabla = {
        {"1", "Curso", &crearCurso},
        {"2", "Diplomado", &crearDiplomado},
        {"3", "Bootcamp", &crearBootcamp},
    };
    return tabla;
}

// Busca y devuelve el nombre del programa asociado al numero del menu
std::string obtenerNombreProgramaPorOpcion(const std::string& opcion) {
    const std::string buscada = recortar(opcion);
    const std::vector<OpcionPrograma>& tabla = programasDisponibles();
    for (std::size_t i = 0; i < tabla.size(); ++i) {
        if (tabla[i].opcion == buscada) {
            return tabla[i].nombre;
        }
    }
    return "";
}

// Fabrica: crea en memoria (con new) el programa segun el nombre indicado
ProgramaAcademico* crearPrograma(const std::string& nombre) {
    const std::string buscado = aMinusculas(recortar(nombre));
    const std::vector<OpcionPrograma>& tabla = programasDisponibles();
    for (std::size_t i = 0; i < tabla.size(); ++i) {
        if (aMinusculas(tabla[i].nombre) == buscado) {
            return tabla[i].crear();
        }
    }
    throw std::invalid_argument("Programa no valido: '" + nombre + "'.");
}


// Constructor de la clase base Persona: asigna datos comunes
Persona::Persona(const std::string& cedula, const std::string& nombre, const std::string& correo)
    : cedula_(cedula), nombre_(nombre), correo_(correo) {}

// Constructor de Alumno: guarda sus datos y le asigna el programa academico
Alumno::Alumno(const std::string& cedula, const std::string& nombre, const std::string& correo,
               ProgramaAcademico* programa)
    : Persona(cedula, nombre, correo), programa_(programa), notas_() {}

// Destructor de Alumno: borra el programa asignado para liberar la memoria
Alumno::~Alumno() {
    delete programa_;
}

// Agrega una nota al alumno si esta en el rango valido de 0 a 20
void Alumno::agregarNota(double nota) {
    if (!(nota >= 0 && nota <= 20)) {
        throw std::invalid_argument("La nota debe estar entre 0 y 20.");
    }
    notas_.push_back(nota);
}

// Borra la ultima nota guardada y la devuelve
double Alumno::quitarUltimaNota() {
    if (notas_.empty()) {
        throw std::invalid_argument("El alumno no tiene notas para quitar.");
    }
    double nota = notas_.back();
    notas_.pop_back();
    return nota;
}

// Le pide al programa asignado que calcule el promedio de las notas
double Alumno::promedio() const {
    return programa_->calcularPromedio(notas_);
}

// Formatea el promedio como texto recortado a 1 decimal
std::string Alumno::promedioTexto() const {
    double truncado = std::floor(promedio() * 10.0) / 10.0;
    std::ostringstream salida;
    salida << std::fixed << std::setprecision(1) << truncado;
    return salida.str();
}

// Revisa si el alumno aprueba (el programa define la regla mediante polimorfismo)
bool Alumno::estaAprobado() const {
    return programa_->evaluarAprobacion(notas_);
}

// Convierte la lista de notas en una cadena separada por comas
std::string Alumno::notasComoTexto() const {
    if (notas_.empty()) {
        return "Sin notas";
    }
    std::string texto;
    for (std::size_t i = 0; i < notas_.size(); ++i) {
        if (i > 0) {
            texto += ", ";
        }
        texto += formatearNota(notas_[i]);
    }
    return texto;
}

// Arma la linea con todos los datos del alumno para guardarla en el archivo .txt
std::string Alumno::aLinea() const {
    return getCedula() + ", " + getNombre() + ", " + getCorreo() + ", " +
           programa_->nombre() + ", " + notasComoTexto();
}

// Constructor de Profesor: inicializa datos personales, especialidad y materia
Profesor::Profesor(const std::string& cedula, const std::string& nombre, const std::string& correo,
                   const std::string& especialidad, const std::string& materia)
    : Persona(cedula, nombre, correo), especialidad_(especialidad), materia_(materia) {}

// Arma la linea con los datos del profesor para guardarla en el archivo .txt
std::string Profesor::aLinea() const {
    return getCedula() + ", " + getNombre() + ", " + getCorreo() + ", " +
           especialidad_ + ", " + materia_;
}
