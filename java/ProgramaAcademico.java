// ProgramaAcademico.java
// Archivos Importados
import java.util.List;

// Clase base abstracta de todos los programas academicos.
// No se puede instanciar: obliga a las hijas a definir sus reglas de aprobacion.
public abstract class ProgramaAcademico {

    // Nombre legible del programa (cada hija lo define)
    public abstract String getNombre();

    // Calcula el promedio de la lista de notas recibidas (0.0 si no hay notas)
    public double calcularPromedio(List<Double> notas) {
        if (notas.isEmpty()) {
            return 0.0;
        }
        double suma = 0.0;
        for (double nota : notas) {
            suma += nota;
        }
        return suma / notas.size();
    }

    // Metodo abstracto que decide si el alumno aprueba.
    public abstract boolean evaluarAprobacion(List<Double> notas);
}
