// Diplomado.java
// Archivos Importados
import java.util.List;


// Diplomado: aprueba si tiene notas y el promedio es >= 14
public class Diplomado extends ProgramaAcademico {

    @Override
    public String getNombre() {
        return "Diplomado";
    }

    // Evalua si el alumno aprueba el diplomado (promedio mayor o igual a 14)
    @Override
    public boolean evaluarAprobacion(List<Double> notas) {
         // Aca es donde se evalua las 3 notas obligatorias y promedio minimo
        if (notas.size() < getNotasRequeridas()) {
            return false;
        }
        return calcularPromedio(notas) >= 14.0;
    }
}
