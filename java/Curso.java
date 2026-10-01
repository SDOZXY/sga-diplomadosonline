// Curso.java
// Archivos Importados
import java.util.List;


// Curso: aprueba si tiene notas y el promedio es >= 10
public class Curso extends ProgramaAcademico {

    @Override
    public String getNombre() {
        return "Curso";
    }

    // Evalua si el alumno aprueba el curso (promedio mayor o igual a 10)
    @Override
    public boolean evaluarAprobacion(List<Double> notas) {
        return !notas.isEmpty() && calcularPromedio(notas) >= 10.0;
    }
}
