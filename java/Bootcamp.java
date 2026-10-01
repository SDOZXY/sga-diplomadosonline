// Bootcamp.java
// Archivos Importados
import java.util.List;


// Exige todas las notas y que Ninguna sea menor a 14.
public class Bootcamp extends ProgramaAcademico {

    @Override
    public String getNombre() {
        return "Bootcamp";
    }

    @Override
    public boolean evaluarAprobacion(List<Double> notas) {
        if (notas.isEmpty()) {
            return false;
        }
        for (double nota : notas) {
            if (nota < 14.0) {
                return false;
            }
        }
        return true;
    }
}
