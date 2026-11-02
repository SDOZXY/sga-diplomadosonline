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
       // Exige tener las 3 notas obligatorias
        if (notas.size() < getNotasRequeridas()) {
            return false;
        }
        // Exige que ninguna nota sea inferior a 14.0
        for (double nota : notas) {
            if (nota < 14.0) {
                return false;
            }
        }
        return true;
    }
}
