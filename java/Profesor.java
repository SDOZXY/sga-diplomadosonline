// Profesor.java
// Archivos Importados
public class Profesor extends Persona {

    private final String especialidad;
    private final String materia;

    // Inicializa los datos de Persona junto con la especialidad y materia
    public Profesor(String cedula, String nombre, String correo, String especialidad, String materia) {
        super(cedula, nombre, correo);
        this.especialidad = especialidad;
        this.materia = materia;
    }

    public String getEspecialidad() {
        return especialidad;
    }

    public String getMateria() {
        return materia;
    }

    // Genera la linea que se guarda en profesores.txt
    public String aLinea() {
        return getCedula() + ", " + getNombre() + ", " + getCorreo() + ", "
                + especialidad + ", " + materia;
    }
}
