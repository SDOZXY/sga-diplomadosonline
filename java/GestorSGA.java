// GestorSGA.java
// Archivos Importados
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Scanner;

// Clase principal que gestiona los datos y archivos del sistema
public class GestorSGA {

    // Nombres de los archivos de texto
    private static final String ARCH_ALUMNOS = "alumnos.txt";
    private static final String ARCH_PROFESORES = "profesores.txt";
    private static final String ARCH_CERTIFICADOS = "certificados_pendientes.txt";

    // Estructuras para guardar datos en memoria
    private final Map<String, Alumno> alumnos = new LinkedHashMap<>();
    private final Map<String, Profesor> profesores = new LinkedHashMap<>();
    private final Pila<String> pilaDeshacer = new Pila<>();
    private int lineasIgnoradas = 0;

    // Clase interna para devolver la nota deshecha
    public static class ResultadoDeshacer {
        private final Alumno alumno;
        private final double nota;

        public ResultadoDeshacer(Alumno alumno, double nota) {
            this.alumno = alumno;
            this.nota = nota;
        }

        public Alumno getAlumno() { return alumno; }
        public double getNota() { return nota; }
    }

    // Carga los datos al iniciar el gestor
    public GestorSGA() throws SgaException {
        cargarDatos();
    }

    public int getLineasIgnoradas() {
        return lineasIgnoradas;
    }

    // Limpia espacios y elimina comas del texto
    private static String limpiar(String texto, String campo) throws SgaException {
        if (texto == null || texto.trim().isEmpty()) {
            throw new SgaException("El campo '" + campo + "' no puede estar vacio.");
        }
        return texto.replace(",", " ").trim();
    }

    // Valida que el correo tenga arroba y punto
    private static void validarCorreo(String correo) throws SgaException {
        if (correo == null || !correo.contains("@") || !correo.contains(".")) {
            throw new SgaException("El correo no parece valido (ejemplo: ana@email.com).");
        }
    }

    // Busca un alumno por su cedula
    private Alumno buscarAlumno(String cedula) throws SgaException {
        Alumno alumno = alumnos.get(limpiar(cedula, "Cedula"));
        if (alumno == null) {
            throw new SgaException("No existe un alumno con esa cedula.");
        }
        return alumno;
    }

    // Escribe una lista de lineas en un archivo
    private static void escribirArchivo(String nombreArchivo, List<String> lineas) throws SgaException {
        PrintWriter pw = null;
        try {
            pw = new PrintWriter(new FileWriter(nombreArchivo));
            for (String linea : lineas) {
                pw.println(linea);
            }
        } catch (IOException e) {
            throw new SgaException("No se pudo guardar el archivo " + nombreArchivo + ".", e);
        } finally {
            if (pw != null) {
                pw.close();
            }
        }
    }

    // Lee las lineas de un archivo de texto
    private static List<String> leerArchivo(String nombreArchivo) throws SgaException {
        List<String> lineas = new ArrayList<>();
        File archivo = new File(nombreArchivo);
        if (!archivo.exists()) {
            return lineas;
        }
        Scanner lector = null;
        try {
            lector = new Scanner(archivo, "UTF-8");
            while (lector.hasNextLine()) {
                lineas.add(lector.nextLine());
            }
        } catch (IOException e) {
            throw new SgaException("No se pudo leer el archivo " + nombreArchivo + ".", e);
        } finally {
            if (lector != null) {
                lector.close();
            }
        }
        return lineas;
    }

    // Guarda los alumnos en el archivo
    public void guardarAlumnos() throws SgaException {
        List<String> lineas = new ArrayList<>();
        for (Alumno alumno : alumnos.values()) {
            lineas.add(alumno.aLinea());
        }
        escribirArchivo(ARCH_ALUMNOS, lineas);
    }

    // Guarda los profesores en el archivo
    public void guardarProfesores() throws SgaException {
        List<String> lineas = new ArrayList<>();
        for (Profesor profesor : profesores.values()) {
            lineas.add(profesor.aLinea());
        }
        escribirArchivo(ARCH_PROFESORES, lineas);
    }

    // Carga los archivos al abrir el programa
    private void cargarDatos() throws SgaException {
        for (String linea : leerArchivo(ARCH_ALUMNOS)) {
            if (!linea.trim().isEmpty()) {
                cargarLineaAlumno(linea);
            }
        }
        for (String linea : leerArchivo(ARCH_PROFESORES)) {
            if (!linea.trim().isEmpty()) {
                cargarLineaProfesor(linea);
            }
        }
    }

    // Procesa cada linea de alumno leida del archivo
    private void cargarLineaAlumno(String linea) {
        try {
            String[] p = linea.split(",");
            if (p.length < 4) {
                lineasIgnoradas++;
                return;
            }
            String cedula = p[0].trim();
            String nombre = p[1].trim();
            String correo = p[2].trim();
            String progNombre = p[3].trim();

            Alumno alumno = new Alumno(cedula, nombre, correo, Programas.crearPrograma(progNombre));
            for (int i = 4; i < p.length; i++) {
                String val = p[i].trim();
                if (!val.isEmpty() && !val.equals("Sin notas") && !val.equals("-")) {
                    alumno.agregarNota(Double.parseDouble(val));
                }
            }
            alumnos.put(cedula, alumno);
        } catch (Exception e) {
            lineasIgnoradas++;
        }
    }

    // Procesa cada linea de profesor leida del archivo
    private void cargarLineaProfesor(String linea) {
        try {
            String[] p = linea.split(",");
            if (p.length == 5) {
                String cedula = p[0].trim();
                Profesor prof = new Profesor(cedula, p[1].trim(), p[2].trim(), p[3].trim(), p[4].trim());
                profesores.put(cedula, prof);
            } else {
                lineasIgnoradas++;
            }
        } catch (Exception e) {
            lineasIgnoradas++;
        }
    }

    // Registra un nuevo alumno y lo guarda en disco
    public void registrarAlumno(String cedula, String nombre, String correo, String nombrePrograma) throws SgaException {
        cedula = limpiar(cedula, "Cedula");
        nombre = limpiar(nombre, "Nombre");
        correo = limpiar(correo, "Correo");
        validarCorreo(correo);
        if (alumnos.containsKey(cedula)) {
            throw new SgaException("Ya existe un alumno con esa cedula.");
        }
        ProgramaAcademico programa = Programas.crearPrograma(nombrePrograma);
        alumnos.put(cedula, new Alumno(cedula, nombre, correo, programa));
        guardarAlumnos();
    }

    // Registra un nuevo profesor y lo guarda en disco
    public void registrarProfesor(String cedula, String nombre, String correo, String especialidad, String materia) throws SgaException {
        cedula = limpiar(cedula, "Cedula");
        nombre = limpiar(nombre, "Nombre");
        correo = limpiar(correo, "Correo");
        validarCorreo(correo);
        especialidad = limpiar(especialidad, "Especialidad");
        materia = limpiar(materia, "Materia");
        if (profesores.containsKey(cedula)) {
            throw new SgaException("Ya existe un profesor con esa cedula.");
        }
        profesores.put(cedula, new Profesor(cedula, nombre, correo, especialidad, materia));
        guardarProfesores();
    }

    // Agrega una nota a un alumno y guarda los cambios
    public Alumno registrarNota(String cedula, double nota) throws SgaException {
        Alumno alumno = buscarAlumno(cedula);
        alumno.agregarNota(nota);
        pilaDeshacer.apilar(alumno.getCedula());
        guardarAlumnos();
        return alumno;
    }

    // Elimina la ultima nota agregada usando la pila
    public ResultadoDeshacer deshacerNota() throws SgaException {
        if (pilaDeshacer.estaVacia()) {
            throw new SgaException("No hay notas para deshacer.");
        }
        String cedula = pilaDeshacer.desapilar();
        Alumno alumno = alumnos.get(cedula);
        double nota = alumno.quitarUltimaNota();
        guardarAlumnos();
        return new ResultadoDeshacer(alumno, nota);
    }

    // Genera el archivo con los alumnos aprobados usando una cola
    public int generarCertificados() throws SgaException {
        Cola<Alumno> cola = new Cola<>();
        for (Alumno alumno : alumnos.values()) {
            if (alumno.estaAprobado()) {
                cola.encolar(alumno);
            }
        }
        int total = cola.tamano();
        List<String> lineas = new ArrayList<>();
        lineas.add("=== REPORTE DE CERTIFICADOS PENDIENTES ===");
        lineas.add("Total de graduandos en cola: " + total);
        lineas.add("");
        int posicion = 1;
        while (!cola.estaVacia()) {
            Alumno alumno = cola.desencolar();
            lineas.add(posicion + ". [" + alumno.getCedula() + "] " + alumno.getNombre());
            lineas.add("   - Programa: " + alumno.getPrograma().getNombre());
            lineas.add("   - Promedio Final: " + alumno.promedioTexto());
            lineas.add("   - Estatus: APROBADO");
            lineas.add("");
            posicion++;
        }
        escribirArchivo(ARCH_CERTIFICADOS, lineas);
        return total;
    }

    // Devuelve los profesores ordenados por cedula
    public List<Profesor> getProfesoresOrdenados() {
        List<Profesor> lista = new ArrayList<>(profesores.values());
        Collections.sort(lista, new Comparator<Profesor>() {
            @Override
            public int compare(Profesor p1, Profesor p2) {
                return p1.getCedula().compareTo(p2.getCedula());
            }
        });
        return lista;
    }

    // Devuelve los alumnos ordenados por cedula
    public List<Alumno> getAlumnosOrdenados() {
        List<Alumno> lista = new ArrayList<>(alumnos.values());
        Collections.sort(lista, new Comparator<Alumno>() {
            @Override
            public int compare(Alumno a1, Alumno a2) {
                return a1.getCedula().compareTo(a2.getCedula());
            }
        });
        return lista;
    }

    // Guarda todos los archivos del sistema
    public void guardarTodo() throws SgaException {
        guardarAlumnos();
        guardarProfesores();
    }
}
