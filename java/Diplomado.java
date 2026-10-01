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
        return !notas.isEmpty() && calcularPromedio(notas) >= 14.0;
    }
}
