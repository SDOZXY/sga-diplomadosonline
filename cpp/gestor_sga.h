// gestor_sga.h 
// archivos importados
#ifndef GESTOR_SGA_H
#define GESTOR_SGA_H
#include <string>
#include <utility>
#include <vector>
#include "data_structures.h"
#include "models.h"


class GestorSGA {
private:
    // Lista de punteros que guardan a los alumnos y profesores en memoria.
    // El gestor administra la memoria: los crea con new y los borra con delete.
    std::vector<Alumno*> alumnos_;
    std::vector<Profesor*> profesores_;

    // Pila (LIFO) para guardar las cedulas de los alumnos segun se registran sus notas (para deshacer)
    Pila<std::string> pilaDeshacer_;

    // Contador de lineas que tenian errores al leer los archivos guardados
    int lineasIgnoradas_;

    // Funciones auxiliares para validar datos y buscar personas por cedula
    static std::string limpiar(const std::string& texto, const std::string& campo);
    static void validarCorreo(const std::string& correo);
    Alumno* buscarAlumnoExacto(const std::string& cedula) const;
    Profesor* buscarProfesorExacto(const std::string& cedula) const;
    Alumno& buscarAlumno(const std::string& cedula) const;
    void almacenarAlumno(Alumno* alumno);
    void almacenarProfesor(Profesor* profesor);

    // Funciones para cargar los datos guardados en los archivos de texto
    void cargarDatos();
    void cargarLineaAlumno(const std::string& linea);
    void cargarLineaProfesor(const std::string& linea);

public:
    // Constructor: inicializa el gestor y lee los datos existentes de los archivos
    GestorSGA();

    // Destructor: libera toda la memoria dinamica para evitar fugas (fugas de memoria)
    ~GestorSGA();

    // Se deshabilita la copia del gestor para no duplicar los punteros
    GestorSGA(const GestorSGA&) = delete;
    GestorSGA& operator=(const GestorSGA&) = delete;

    // Devuelve el numero de lineas ignoradas por estar corruptas
    int getLineasIgnoradas() const { return lineasIgnoradas_; }

    // Reescriben la informacion actualizada en los archivos .txt correspondientes
    void guardarAlumnos() const;
    void guardarProfesores() const;

    // Opcion 1: Valida los datos e inscribe un alumno nuevo
    void registrarAlumno(const std::string& cedula, const std::string& nombre,
                         const std::string& correo, const std::string& nombrePrograma);

    // Opcion 2: Valida los datos e inscribe un profesor nuevo
    void registrarProfesor(const std::string& cedula, const std::string& nombre,
                           const std::string& correo, const std::string& especialidad,
                           const std::string& materia);

    // Opcion 3: Agrega una nota al alumno, la guarda en la pila de deshacer y actualiza el archivo
    const Alumno& registrarNota(const std::string& cedula, double nota);

    // Opcion 4: Quita la ultima nota agregada usando la pila (LIFO)
    std::pair<std::string, double> deshacerNota();

    // Opcion 5: Filtra alumnos aprobados, los encola (FIFO) y genera el archivo de certificados
    std::size_t generarCertificados();

    // Opcion 6: Retorna las listas de alumnos y profesores ordenados por cedula para mostrarlos en consola
    std::vector<const Profesor*> getProfesoresOrdenados() const;
    std::vector<const Alumno*> getAlumnosOrdenados() const;

    // Opcion 7: Guarda todos los datos en los archivos antes de cerrar la aplicacion
    void guardarTodo() const;
};

#endif  // GESTOR_SGA_H
