// Main.java (equivale a main.py)
// Archivos Importados
import java.io.PrintStream;
import java.io.UnsupportedEncodingException;
import java.util.List;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.Scanner;

public final class Main {

    // Lector de teclado compartido por todo el programa
    private static final Scanner ENTRADA = new Scanner(System.in, "UTF-8");

    // Mensaje unico para cualquier dato numerico invalido
    private static final String ERROR_NUMERICO = "Error: Ingrese un valor numerico valido";

    private Main() {
    }

    // Pide un texto por teclado y elimina espacios al inicio y final.
    // Si se cierra la entrada (Ctrl+Z / Ctrl+D) Scanner lanza NoSuchElementException,
    // que se captura en main().
    private static String pedirTexto(String mensaje) {
        System.out.print(mensaje);
        return ENTRADA.nextLine().trim();
    }

    // Pide un numero permitiendo coma o punto decimal. Devuelve null si el dato no es valido
    private static Double pedirNota(String mensaje) {
        try {
            double valor = Double.parseDouble(pedirTexto(mensaje).replace(",", "."));
            if (Double.isNaN(valor) || Double.isInfinite(valor)) {
                throw new NumberFormatException();
            }
            return valor;
        } catch (NumberFormatException e) {
            System.out.println("  -> " + ERROR_NUMERICO);
            return null;
        }
    }

    // Muestra los programas disponibles y devuelve el nombre elegido o null si es invalido
    private static String pedirPrograma() {
        System.out.println("Programas disponibles:");
        for (Map.Entry<String, String> opcion : Programas.getOpciones().entrySet()) {
            System.out.println("  " + opcion.getKey() + ". " + opcion.getValue());
        }
        String nombre = Programas.obtenerNombreProgramaPorOpcion(pedirTexto("Elige el programa: "));
        if (nombre == null) {
            System.out.println("  -> Opcion de programa no valida.");
        }
        return nombre;
    }

    // Opcion 1: pide los datos del alumno y se los entrega al gestor para registrarlo
    private static void opcionRegistrarAlumno(GestorSGA gestor) throws SgaException {
        String cedula = pedirTexto("Cedula: ");
        String nombre = pedirTexto("Nombre: ");
        String correo = pedirTexto("Correo: ");
        String programa = pedirPrograma();
        if (programa == null) {
            return;
        }
        gestor.registrarAlumno(cedula, nombre, correo, programa);
        System.out.println("  -> Alumno registrado y guardado en alumnos.txt.");
    }

    // Opcion 2: pide los datos del profesor y se los entrega al gestor para registrarlo
    private static void opcionRegistrarProfesor(GestorSGA gestor) throws SgaException {
        String cedula = pedirTexto("Cedula: ");
        String nombre = pedirTexto("Nombre: ");
        String correo = pedirTexto("Correo: ");
        String especialidad = pedirTexto("Especialidad: ");
        String materia = pedirTexto("Materia: ");
        gestor.registrarProfesor(cedula, nombre, correo, especialidad, materia);
        System.out.println("  -> Profesor registrado y guardado en profesores.txt.");
    }

    // Opcion 3: pide cedula y nota del alumno para registrarla en el sistema
    private static void opcionRegistrarNota(GestorSGA gestor) throws SgaException {
        String cedula = pedirTexto("Cedula del alumno: ");
        Double nota = pedirNota("Nota (0-20): ");
        if (nota == null) {
            return;
        }
        Alumno alumno = gestor.registrarNota(cedula, nota);
        int cantidad = alumno.getNotas().size();
        String texto = cantidad == 1 ? "nota registrada" : "notas registradas";
        System.out.println("  -> Nota " + Alumno.formatearNota(nota) + " registrada a "
                + alumno.getNombre() + " (" + cantidad + " " + texto + ").");
    }

    // Opcion 4: deshace la ultima nota registrada utilizando la pila (LIFO)
    private static void opcionDeshacerNota(GestorSGA gestor) throws SgaException {
        GestorSGA.ResultadoDeshacer resultado = gestor.deshacerNota();
        System.out.println("  -> Se elimino la nota " + Alumno.formatearNota(resultado.getNota())
                + " de " + resultado.getAlumno().getNombre() + ".");
    }

    // Opcion 5: genera la cola de alumnos aprobados y exporta el reporte de certificados
    private static void opcionGenerarCertificados(GestorSGA gestor) throws SgaException {
        int total = gestor.generarCertificados();
        System.out.println("  -> " + total + " graduando(s) exportados a certificados_pendientes.txt.");
    }

    // Opcion 6: imprime en consola la lista de profesores y alumnos registrados
    private static void opcionMostrarReporte(GestorSGA gestor) {
        List<Profesor> profesores = gestor.getProfesoresOrdenados();
        List<Alumno> alumnos = gestor.getAlumnosOrdenados();

        System.out.println("\n--- PROFESORES ACTIVOS ---");
        if (profesores.isEmpty()) {
            System.out.println("(sin profesores registrados)");
        }
        for (Profesor p : profesores) {
            System.out.println("[" + p.getCedula() + "] " + p.getNombre() + " | "
                    + p.getEspecialidad() + " | " + p.getMateria());
        }

        System.out.println("\n--- ALUMNOS ---");
        if (alumnos.isEmpty()) {
            System.out.println("(sin alumnos registrados)");
        }
        for (Alumno a : alumnos) {
            String estatus = a.estaAprobado() ? "APROBADO" : "REPROBADO";
            System.out.println("[" + a.getCedula() + "] " + a.getNombre() + " | "
                    + a.getPrograma().getNombre() + " | Notas: "
                    + (a.getNotas().isEmpty() ? "sin notas" : a.notasComoTexto())
                    + " | Promedio: " + a.promedioTexto() + " | " + estatus);
        }
    }

    // Imprime las opciones del menu principal en consola
    private static void mostrarMenu() {
        System.out.println("\n========== SGA - Diplomados Online ==========");
        System.out.println("1. Registrar Alumno");
        System.out.println("2. Registrar Profesor");
        System.out.println("3. Registrar Notas a un Alumno");
        System.out.println("4. Deshacer Ultimo Registro de Nota");
        System.out.println("5. Generar Cola de Certificados");
        System.out.println("6. Mostrar Reporte General");
        System.out.println("7. Salir");
    }

    // Lee la opcion elegida y valida que sea un entero. Si el usuario escribe letras
    // o solo presiona Enter, muestra el aviso y devuelve null (se vuelve al menu).
    private static Integer leerOpcion() {
        try {
            return Integer.parseInt(pedirTexto("Elige una opcion: "));
        } catch (NumberFormatException e) {
            System.out.println("  -> " + ERROR_NUMERICO);
            return null;
        }
    }

    // Ejecuta la opcion elegida. Devuelve false cuando el usuario decide salir.
    private static boolean ejecutarOpcion(int opcion, GestorSGA gestor) throws SgaException {
        switch (opcion) {
            case 1:
                opcionRegistrarAlumno(gestor);
                return true;
            case 2:
                opcionRegistrarProfesor(gestor);
                return true;
            case 3:
                opcionRegistrarNota(gestor);
                return true;
            case 4:
                opcionDeshacerNota(gestor);
                return true;
            case 5:
                opcionGenerarCertificados(gestor);
                return true;
            case 6:
                opcionMostrarReporte(gestor);
                return true;
            case 7:
                gestor.guardarTodo();
                System.out.println("Datos guardados. Hasta luego.");
                return false;
            default:
                System.out.println("  -> Opcion fuera de rango. Elige un numero del 1 al 7.");
                return true;
        }
    }

    // Configura la consola en UTF-8 para que se vean bien las tildes
    // (en Windows ejecutar antes: chcp 65001)
    private static void configurarConsola() {
        try {
            System.setOut(new PrintStream(System.out, true, "UTF-8"));
        } catch (UnsupportedEncodingException e) {
            // UTF-8 siempre existe en Java; si fallara se usa la consola por defecto
        }
    }

    // Funcion principal: ejecuta el bucle e interactua con el usuario
    public static void main(String[] args) {
        configurarConsola();

        GestorSGA gestor;
        try {
            gestor = new GestorSGA();
        } catch (SgaException e) {
            System.out.println("  -> " + e.getMessage());
            return;
        }
        if (gestor.getLineasIgnoradas() > 0) {
            System.out.println("Aviso: se ignoraron " + gestor.getLineasIgnoradas()
                    + " linea(s) danada(s) al cargar los archivos.");
        }

        boolean continuar = true;
        while (continuar) {
            mostrarMenu();
            try {
                Integer opcion = leerOpcion();
                if (opcion != null) {
                    continuar = ejecutarOpcion(opcion, gestor);
                }
            } catch (SgaException e) {
                // Errores de validacion lanzados por el gestor
                System.out.println("  -> " + e.getMessage());
            } catch (NoSuchElementException e) {
                // La entrada se cerro (Ctrl+Z / Ctrl+D): los datos ya estaban guardados
                System.out.println("\nSaliendo... los datos ya estaban guardados.");
                continuar = false;
            }
        }
    }
}
