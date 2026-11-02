// SgaException.java
// Archivos Importados
public class SgaException extends Exception {

    private static final long serialVersionUID = 1L;

    // Crea la excepcion con un mensaje amable para mostrar al usuario
    public SgaException(String mensaje) {
        super(mensaje);
    }

    // Crea la excepcion conservando la causa original (por ejemplo un IOException)
    public SgaException(String mensaje, Throwable causa) {
        super(mensaje, causa);
    }
}
