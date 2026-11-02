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
        // Aca es donde se evalua las 3 notas obligatorias y promedio minimo
        if (notas.size() < getNotasRequeridas()) {
            return false;
        }
        return calcularPromedio(notas) >= 10.0;
    }
}
