// Programas.java
// Archivos Importados
import java.util.LinkedHashMap;
import java.util.Map;

// Clase para crear los objetos de los programas academicos
public final class Programas {

    // Constructor privado para evitar instancias
    private Programas() {
    }

    // Devuelve la lista de opciones para el menu
    public static Map<String, String> getOpciones() {
        Map<String, String> opciones = new LinkedHashMap<>();
        opciones.put("1", "Curso");
        opciones.put("2", "Diplomado");
        opciones.put("3", "Bootcamp");
        return opciones;
    }

    // Retorna el nombre del programa segun el numero elegido
    public static String obtenerNombreProgramaPorOpcion(String opcion) {
        if (opcion == null) return null;
        switch (opcion.trim()) {
            case "1": return "Curso";
            case "2": return "Diplomado";
            case "3": return "Bootcamp";
            default: return null;
        }
    }

    // Instancia el objeto del programa correspondiente
    public static ProgramaAcademico crearPrograma(String nombre) throws SgaException {
        if (nombre == null) {
            throw new SgaException("El nombre del programa no puede ser nulo.");
        }
        String buscado = nombre.trim();
        if (buscado.equalsIgnoreCase("Curso")) {
            return new Curso();
        } else if (buscado.equalsIgnoreCase("Diplomado")) {
            return new Diplomado();
        } else if (buscado.equalsIgnoreCase("Bootcamp")) {
            return new Bootcamp();
        } else {
            throw new SgaException("Programa no valido: '" + nombre + "'.");
        }
    }
}
