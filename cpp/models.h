// models.h
// Archivos Importados
#ifndef MODELS_H
#define MODELS_H
#include <cstddef>
#include <string>
#include <vector>


// Quita los espacios vacios al inicio y al final de una palabra o frase
std::string recortar(const std::string& texto);


// Convierte un texto a numero decimal. Devuelve false si no es un numero valido
bool convertirADouble(const std::string& texto, double& resultado);


// Le da formato bonito a la nota para mostrarla en pantalla
std::string formatearNota(double nota);


// Clase base para cualquier programa. Define la plantilla general
class ProgramaAcademico {
public:
    // Destructor virtual para limpiar la memoria correctamente
    virtual ~ProgramaAcademico() {}

    // Devuelve el nombre del programa
    virtual std::string nombre() const = 0;

    // Saca el promedio de una lista de notas
    double calcularPromedio(const std::vector<double>& notas) const;

    // Regla de aprobacion (cada tipo de programa la calcula a su manera)
    virtual bool evaluarAprobacion(const std::vector<double>& notas) const = 0;
};

// Programa tipo Curso (aprueba con promedio >= 10)
class Curso : public ProgramaAcademico {
public:
    std::string nombre() const override;
    bool evaluarAprobacion(const std::vector<double>& notas) const override;
};

// Programa tipo Diplomado (aprueba con promedio >= 14)
class Diplomado : public ProgramaAcademico {
public:
    std::string nombre() const override;
    bool evaluarAprobacion(const std::vector<double>& notas) const override;
};

// Programa tipo Bootcamp (pide 3 notas y que ninguna sea menor a 14)
class Bootcamp : public ProgramaAcademico {
public:
    std::string nombre() const override;
    bool evaluarAprobacion(const std::vector<double>& notas) const override;
};

// Estructura para organizar las opciones del menu
struct OpcionPrograma {
    std::string opcion;
    std::string nombre;
    ProgramaAcademico* (*crear)(); // Funcion que crea el programa en memoria
};

// Lista de todos los programas disponibles en el sistema
const std::vector<OpcionPrograma>& programasDisponibles();

// Busca el nombre del programa segun la opcion elegida
std::string obtenerNombreProgramaPorOpcion(const std::string& opcion);

// Crea un programa nuevo segun el nombre recibido
ProgramaAcademico* crearPrograma(const std::string& nombre);

// Clase base con los datos generales de una persona
class Persona {
private:
    std::string cedula_;
    std::string nombre_;
    std::string correo_;

public:
    Persona(const std::string& cedula, const std::string& nombre, const std::string& correo);
    virtual ~Persona() {}

    // Métodos para consultar los datos
    const std::string& getCedula() const { return cedula_; }
    const std::string& getNombre() const { return nombre_; }
    const std::string& getCorreo() const { return correo_; }
};

// Clase para Alumnos (hereda de Persona)
class Alumno : public Persona {
private:
    ProgramaAcademico* programa_; // Guarda el programa asignado
    std::vector<double> notas_;   // Lista donde se guardan sus notas

public:
    Alumno(const std::string& cedula, const std::string& nombre, const std::string& correo,
           ProgramaAcademico* programa);

    // Libera la memoria del programa al borrar al alumno
    ~Alumno() override;

    // Se bloquea la copia para evitar errores de memoria duplicada
    Alumno(const Alumno&) = delete;
    Alumno& operator=(const Alumno&) = delete;

    const ProgramaAcademico& getPrograma() const { return *programa_; }
    const std::vector<double>& getNotas() const { return notas_; }

    // Agrega una nueva nota al alumno (valida maximo 3 notas entre 0 y 20)
    void agregarNota(double nota);

    // Borra la ultima nota ingresada
    double quitarUltimaNota();

    // Calcula el promedio actual
    double promedio() const;

    // Devuelve el promedio como texto corto
    std::string promedioTexto() const;

    // Revisa si el alumno esta aprobado segun las reglas de su programa
    bool estaAprobado() const;

    // Devuelve todas las notas juntas en un texto
    std::string notasComoTexto() const;

    // Convierte los datos a texto para guardarlos en el archivo de texto (.txt)
    std::string aLinea() const;
};

// Clase para Profesores (hereda de Persona)
class Profesor : public Persona {
private:
    std::string especialidad_;
    std::string materia_;

public:
    Profesor(const std::string& cedula, const std::string& nombre, const std::string& correo,
             const std::string& especialidad, const std::string& materia);

    const std::string& getEspecialidad() const { return especialidad_; }
    const std::string& getMateria() const { return materia_; }

    // Convierte los datos a texto para guardarlos en el archivo de profesores (.txt)
    std::string aLinea() const;
};

#endif // MODELS_H
