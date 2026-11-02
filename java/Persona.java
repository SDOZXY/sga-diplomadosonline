// Persona.java
// Archivos Importados
public class Persona {

    private final String cedula;
    private final String nombre;
    private final String correo;

    // Guarda cedula, nombre y correo de una persona
    public Persona(String cedula, String nombre, String correo) {
        this.cedula = cedula;
        this.nombre = nombre;
        this.correo = correo;
    }

    public String getCedula() {
        return cedula;
    }

    public String getNombre() {
        return nombre;
    }

    public String getCorreo() {
        return correo;
    }
}
