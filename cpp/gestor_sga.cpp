// gestor_sga.cpp
// archivos importados
#include "gestor_sga.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>


namespace {

// Nombre de los archivos TXT donde se guarda la informacion
const char* const ARCH_ALUMNOS = "alumnos.txt";
const char* const ARCH_PROFESORES = "profesores.txt";
const char* const ARCH_CERTIFICADOS = "certificados_pendientes.txt";

// Separa una linea de texto por comas y le quita los espacios a cada parte
std::vector<std::string> dividir(const std::string& linea) {
    std::vector<std::string> partes;
    std::string actual;
    for (std::string::size_type i = 0; i < linea.size(); ++i) {
        if (linea[i] == ',') {
            partes.push_back(recortar(actual));
            actual.clear();
        } else {
            actual += linea[i];
        }
    }
    partes.push_back(recortar(actual));
    return partes;
}

// Lee un archivo texto completo linea por linea
std::vector<std::string> leerArchivo(const std::string& ruta) {
    std::vector<std::string> lineas;
    std::ifstream archivo(ruta.c_str());
    if (!archivo) {
        return lineas; // Si el archivo no existe, devuelve la lista vacia
    }
    std::string linea;
    while (std::getline(archivo, linea)) {
        lineas.push_back(linea);
    }
    return lineas;
}

// Guarda o sobrescribe una lista de lineas en un archivo .txt
void escribirArchivo(const std::string& ruta, const std::vector<std::string>& lineas) {
    std::ofstream archivo(ruta.c_str(), std::ios::out | std::ios::trunc);
    if (!archivo) {
        throw std::runtime_error("No se pudo guardar el archivo " + ruta + ".");
    }
    for (std::size_t i = 0; i < lineas.size(); ++i) {
        archivo << lineas[i] << '\n';
    }
    archivo.close();
    if (archivo.fail()) {
        throw std::runtime_error("No se pudo guardar el archivo " + ruta + ".");
    }
}

// Crea un objeto Alumno de forma segura; si falla, borra el programa para no perder memoria
Alumno* crearAlumno(const std::string& cedula, const std::string& nombre,
                    const std::string& correo, ProgramaAcademico* programa) {
    try {
        return new Alumno(cedula, nombre, correo, programa);
    } catch (...) {
        delete programa;
        throw;
    }
}

}  // namespace

// Constructor: inicializa las listas y carga los datos existentes desde los archivos .txt
GestorSGA::GestorSGA() : alumnos_(), profesores_(), pilaDeshacer_(), lineasIgnoradas_(0) {
    cargarDatos();
}

// Destructor: borra todos los objetos creados dinamicamente para liberar memoria
GestorSGA::~GestorSGA() {
    for (std::size_t i = 0; i < alumnos_.size(); ++i) {
        delete alumnos_[i];
    }
    for (std::size_t i = 0; i < profesores_.size(); ++i) {
        delete profesores_[i];
    }
    alumnos_.clear();
    profesores_.clear();
}

// Limpia el texto ingresado, cambia comas por espacios y valida que no quede vacio
std::string GestorSGA::limpiar(const std::string& texto, const std::string& campo) {
    std::string base = texto;
    for (std::string::size_type i = 0; i < base.size(); ++i) {
        if (base[i] == ',') {
            base[i] = ' ';
        }
    }
    std::istringstream flujo(base);
    std::string palabra;
    std::string resultado;
    while (flujo >> palabra) {
        if (!resultado.empty()) {
            resultado += ' ';
        }
        resultado += palabra;
    }
    if (resultado.empty()) {
        throw std::invalid_argument("El campo '" + campo + "' no puede estar vacio.");
    }
    return resultado;
}

// Comprueba que el formato del correo tenga un '@' y un punto posterior
void GestorSGA::validarCorreo(const std::string& correo) {
    std::string::size_type arroba = correo.rfind('@');
    if (arroba == std::string::npos || correo.find('.', arroba + 1) == std::string::npos) {
        throw std::invalid_argument("El correo no parece valido (ejemplo: ana@email.com).");
    }
}

// Busca y retorna el puntero al alumno si la cedula coincide exactamente
Alumno* GestorSGA::buscarAlumnoExacto(const std::string& cedula) const {
    for (std::size_t i = 0; i < alumnos_.size(); ++i) {
        if (alumnos_[i]->getCedula() == cedula) {
            return alumnos_[i];
        }
    }
    return nullptr;
}

// Busca y retorna el puntero al profesor si la cedula coincide exactamente
Profesor* GestorSGA::buscarProfesorExacto(const std::string& cedula) const {
    for (std::size_t i = 0; i < profesores_.size(); ++i) {
        if (profesores_[i]->getCedula() == cedula) {
            return profesores_[i];
        }
    }
    return nullptr;
}

// Busca al alumno por cedula y lanza una excepcion si no se encuentra
Alumno& GestorSGA::buscarAlumno(const std::string& cedula) const {
    Alumno* alumno = buscarAlumnoExacto(limpiar(cedula, "Cedula"));
    if (alumno == nullptr) {
        throw std::invalid_argument("No existe un alumno con esa cedula.");
    }
    return *alumno;
}

// Agrega el alumno a la lista (si la cedula ya existia, reemplaza el objeto anterior)
void GestorSGA::almacenarAlumno(Alumno* alumno) {
    for (std::size_t i = 0; i < alumnos_.size(); ++i) {
        if (alumnos_[i]->getCedula() == alumno->getCedula()) {
            delete alumnos_[i];
            alumnos_[i] = alumno;
            return;
        }
    }
    try {
        alumnos_.push_back(alumno);
    } catch (...) {
        delete alumno;
        throw;
    }
}

// Agrega el profesor a la lista (si la cedula ya existia, reemplaza el objeto anterior)
void GestorSGA::almacenarProfesor(Profesor* profesor) {
    for (std::size_t i = 0; i < profesores_.size(); ++i) {
        if (profesores_[i]->getCedula() == profesor->getCedula()) {
            delete profesores_[i];
            profesores_[i] = profesor;
            return;
        }
    }
    try {
        profesores_.push_back(profesor);
    } catch (...) {
        delete profesor;
        throw;
    }
}


// Guarda la lista completa de alumnos en alumnos.txt
void GestorSGA::guardarAlumnos() const {
    std::vector<std::string> lineas;
    for (std::size_t i = 0; i < alumnos_.size(); ++i) {
        lineas.push_back(alumnos_[i]->aLinea());
    }
    escribirArchivo(ARCH_ALUMNOS, lineas);
}

// Guarda la lista completa de profesores en profesores.txt
void GestorSGA::guardarProfesores() const {
    std::vector<std::string> lineas;
    for (std::size_t i = 0; i < profesores_.size(); ++i) {
        lineas.push_back(profesores_[i]->aLinea());
    }
    escribirArchivo(ARCH_PROFESORES, lineas);
}

// Carga los datos de los archivos al iniciar la aplicacion e ignora lineas corruptas
void GestorSGA::cargarDatos() {
    std::vector<std::string> lineas = leerArchivo(ARCH_ALUMNOS);
    for (std::size_t i = 0; i < lineas.size(); ++i) {
        if (!recortar(lineas[i]).empty()) {
            cargarLineaAlumno(lineas[i]);
        }
    }
    lineas = leerArchivo(ARCH_PROFESORES);
    for (std::size_t i = 0; i < lineas.size(); ++i) {
        if (!recortar(lineas[i]).empty()) {
            cargarLineaProfesor(lineas[i]);
        }
    }
}

// Procesa una linea de texto de alumnos.txt para reconstruir el objeto Alumno
void GestorSGA::cargarLineaAlumno(const std::string& linea) {
    std::vector<std::string> p = dividir(linea);
    if (p.size() < 4 || p[0].empty()) {
        ++lineasIgnoradas_;
        return;
    }

    ProgramaAcademico* programa = nullptr;
    try {
        programa = crearPrograma(p[3]);
    } catch (const std::invalid_argument&) {
        ++lineasIgnoradas_;
        return;
    }

    Alumno* alumno = crearAlumno(p[0], p[1], p[2], programa);
    try {
        for (std::size_t i = 4; i < p.size(); ++i) {
            if (!p[i].empty() && p[i] != "Sin notas" && p[i] != "-") {
                double nota = 0.0;
                if (!convertirADouble(p[i], nota)) {
                    throw std::invalid_argument("nota invalida");
                }
                alumno->agregarNota(nota);
            }
        }
    } catch (const std::invalid_argument&) {
        delete alumno;
        ++lineasIgnoradas_;
        return;
    }
    almacenarAlumno(alumno);
}

// Procesa una linea de texto de profesores.txt para reconstruir el objeto Profesor
void GestorSGA::cargarLineaProfesor(const std::string& linea) {
    std::vector<std::string> p = dividir(linea);
    if (p.size() != 5 || p[0].empty()) {
        ++lineasIgnoradas_;
        return;
    }
    almacenarProfesor(new Profesor(p[0], p[1], p[2], p[3], p[4]));
}


// Opcion 1: Valida los datos, crea el nuevo Alumno y actualiza el archivo TXT
void GestorSGA::registrarAlumno(const std::string& cedulaTexto, const std::string& nombreTexto,
                                const std::string& correoTexto, const std::string& nombrePrograma) {
    const std::string cedula = limpiar(cedulaTexto, "Cedula");
    const std::string nombre = limpiar(nombreTexto, "Nombre");
    const std::string correo = limpiar(correoTexto, "Correo");
    validarCorreo(correo);
    if (buscarAlumnoExacto(cedula) != nullptr) {
        throw std::invalid_argument("Ya existe un alumno con esa cedula.");
    }
    ProgramaAcademico* programa = crearPrograma(nombrePrograma);
    almacenarAlumno(crearAlumno(cedula, nombre, correo, programa));
    guardarAlumnos();
}

// Opcion 2: Valida los datos, crea el nuevo Profesor y actualiza el archivo TXT
void GestorSGA::registrarProfesor(const std::string& cedulaTexto, const std::string& nombreTexto,
                                  const std::string& correoTexto, const std::string& especialidadTexto,
                                  const std::string& materiaTexto) {
    const std::string cedula = limpiar(cedulaTexto, "Cedula");
    const std::string nombre = limpiar(nombreTexto, "Nombre");
    const std::string correo = limpiar(correoTexto, "Correo");
    validarCorreo(correo);
    const std::string especialidad = limpiar(especialidadTexto, "Especialidad");
    const std::string materia = limpiar(materiaTexto, "Materia");
    if (buscarProfesorExacto(cedula) != nullptr) {
        throw std::invalid_argument("Ya existe un profesor con esa cedula.");
    }
    almacenarProfesor(new Profesor(cedula, nombre, correo, especialidad, materia));
    guardarProfesores();
}

// Opcion 3: Asigna una nota al alumno, guarda la accion en la Pila (LIFO) y actualiza el archivo
const Alumno& GestorSGA::registrarNota(const std::string& cedula, double nota) {
    Alumno& alumno = buscarAlumno(cedula);
    alumno.agregarNota(nota);
    pilaDeshacer_.apilar(alumno.getCedula());
    guardarAlumnos();
    return alumno;
}

// Opcion 4: Desapila la ultima cedula (LIFO) y quita la ultima nota ingresada
std::pair<std::string, double> GestorSGA::deshacerNota() {
    if (pilaDeshacer_.estaVacia()) {
        throw std::invalid_argument("No hay notas para deshacer.");
    }
    std::string cedula = pilaDeshacer_.desapilar();
    Alumno* alumno = buscarAlumnoExacto(cedula);
    if (alumno == nullptr) {
        throw std::runtime_error("No se encontro el alumno de la ultima nota registrada.");
    }
    double nota = alumno->quitarUltimaNota();
    guardarAlumnos();
    return std::make_pair(alumno->getNombre(), nota);
}

// Opcion 5: Filtra alumnos aprobados, los procesa en Cola (FIFO) y genera el archivo de certificados
std::size_t GestorSGA::generarCertificados() {
    Cola<Alumno*> cola;
    for (std::size_t i = 0; i < alumnos_.size(); ++i) {
        if (alumnos_[i]->estaAprobado()) {
            cola.encolar(alumnos_[i]);
        }
    }
    const std::size_t total = cola.tamano();

    std::vector<std::string> lineas;
    lineas.push_back("=== REPORTE DE CERTIFICADOS PENDIENTES ===");
    lineas.push_back("Total de graduandos en cola: " + std::to_string(total));
    lineas.push_back("");
    int posicion = 1;
    while (!cola.estaVacia()) {
        const Alumno* alumno = cola.desencolar();
        lineas.push_back(std::to_string(posicion) + ". [" + alumno->getCedula() + "] " +
                         alumno->getNombre());
        lineas.push_back("   - Programa: " + alumno->getPrograma().nombre());
        lineas.push_back("   - Promedio Final: " + alumno->promedioTexto());
        lineas.push_back("   - Estatus: APROBADO");
        lineas.push_back("");
        ++posicion;
    }
    escribirArchivo(ARCH_CERTIFICADOS, lineas);
    return total;
}

// Opcion 6 (parte 1): Devuelve la lista de profesores ordenada por cedula
std::vector<const Profesor*> GestorSGA::getProfesoresOrdenados() const {
    std::vector<const Profesor*> lista(profesores_.begin(), profesores_.end());
    std::sort(lista.begin(), lista.end(), [](const Profesor* a, const Profesor* b) {
        return a->getCedula() < b->getCedula();
    });
    return lista;
}

// Opcion 6 (parte 2): Devuelve la lista de alumnos ordenada por cedula
std::vector<const Alumno*> GestorSGA::getAlumnosOrdenados() const {
    std::vector<const Alumno*> lista(alumnos_.begin(), alumnos_.end());
    std::sort(lista.begin(), lista.end(), [](const Alumno* a, const Alumno* b) {
        return a->getCedula() < b->getCedula();
    });
    return lista;
}

// Opcion 7: Guarda todos los datos en disco de forma segura antes de cerrar el sistema
void GestorSGA::guardarTodo() const {
    guardarAlumnos();
    guardarProfesores();
}
