// Alumno.java
// Archivos Importados
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

// Representa a un alumno registrado en un programa
public class Alumno extends Persona {

    private final ProgramaAcademico programa;
    private final List<Double> notas = new ArrayList<>();

    // Constructor de la clase Alumno
    public Alumno(String cedula, String nombre, String correo, ProgramaAcademico programa) {
        super(cedula, nombre, correo);
        this.programa = programa;
    }

    public ProgramaAcademico getPrograma() {
        return programa;
    }

    public List<Double> getNotas() {
        return Collections.unmodifiableList(notas);
    }

    // Agrega una nota sin limite de cantidad, solo validando el rango
    public void agregarNota(double nota) throws SgaException {
        if (nota < 0 || nota > 20) {
            throw new SgaException("La nota debe estar entre 0 y 20.");
        }
        notas.add(nota);
    }

    // Quita la ultima nota para la funcion deshacer
    public double quitarUltimaNota() throws SgaException {
        if (notas.isEmpty()) {
            throw new SgaException("El alumno no tiene notas para quitar.");
        }
        return notas.remove(notas.size() - 1);
    }

    // Calcula el promedio usando la regla del programa
    public double promedio() {
        return programa.calcularPromedio(notas);
    }

    // Devuelve el promedio en texto recortado
    public String promedioTexto() {
        double prom = Math.floor(promedio() * 10) / 10;
        return String.valueOf(prom);
    }

    // Evalua si el estudiante aprobo el programa
    public boolean estaAprobado() {
        return programa.evaluarAprobacion(notas);
    }

    // Formatea la nota para quitar decimales innecesarios
    public static String formatearNota(double nota) {
        if (nota == (long) nota) {
            return String.valueOf((long) nota);
        }
        return String.valueOf(nota);
    }

    // Une las notas en una sola cadena separada por comas
    public String notasComoTexto() {
        if (notas.isEmpty()) {
            return "Sin notas";
        }
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < notas.size(); i++) {
            sb.append(formatearNota(notas.get(i)));
            if (i < notas.size() - 1) {
                sb.append(", ");
            }
        }
        return sb.toString();
    }

    // Convierte el alumno a formato de linea para guardar en txt
    public String aLinea() {
        return getCedula() + ", " + getNombre() + ", " + getCorreo() + ", "
                + programa.getNombre() + ", " + notasComoTexto();
    }
}
