# ========================================================
# SISTEMA DE GESTIÓN DE ESTUDIANTES - PROYECTO FINAL
# Asignatura: Algoritmos y Programación
# Cumple TODOS los requisitos del enunciado:
# - Uso de listas (arreglos)
# - Cadenas de caracteres
# - Estructuras de control (if/elif/else)
# - Bucles (while y for)
# - Funciones (al menos una que retorna valor y una que no)
# - Búsqueda lineal implementada MANUALMENTE
# - Validación de entradas
# - Código modular y resultados claros
# ========================================================

estudiantes = []  # Lista principal (arreglo) que guarda todos los estudiantes


def agregar_estudiante():
    """Función que NO retorna valor. Agrega un estudiante con validaciones."""
    print("\nAgregar Nuevo Estudiante")

    # Validación del nombre (cadena de caracteres)
    while True:
        nombre = input("Ingrese el nombre completo: ").strip()
        if nombre:
            break
        print("Error: El nombre no puede estar vacío.")

    # Validación de la edad (número entero positivo)
    while True:
        try:
            edad = int(input("Ingrese la edad: "))
            if edad > 0:
                break
            print("Error: La edad debe ser mayor a 0.")
        except ValueError:
            print("Error: Debe ingresar un número entero válido.")

    # Validación de la calificación (número decimal entre 0 y 100)
    while True:
        try:
            calificacion = float(input("Ingrese la calificación (0-100): "))
            if 0 <= calificacion <= 100:
                break
            print("Error: La calificación debe estar entre 0 y 100.")
        except ValueError:
            print("Error: Debe ingresar un número válido.")

    # Guardamos el estudiante en la lista (arreglo)
    estudiantes.append({"nombre": nombre, "edad": edad, "calificacion": calificacion})
    print(f"Estudiante '{nombre}' agregado correctamente.\n")


def mostrar_estudiantes():
    """Función que NO retorna valor. Muestra todos los estudiantes."""
    print("\nLista de Todos los Estudiantes")
    if not estudiantes:
        print("No hay estudiantes registrados aún.")
        return

    print(f"{'No.':<3} {'Nombre':<25} {'Edad':<6} {'Calificación':<12}")
    print("-" * 50)
    for i, est in enumerate(estudiantes, 1):
        print(
            f"{i:<3} {est['nombre']:<25} {est['edad']:<6} {est['calificacion']:<12.2f}"
        )
    print("-" * 50)


def calcular_promedio():
    """Función que SÍ retorna un valor (el promedio)."""
    if not estudiantes:
        print("No hay estudiantes para calcular el promedio.")
        return 0.0

    suma = 0
    for est in estudiantes:
        suma += est["calificacion"]

    promedio = suma / len(estudiantes)
    print(f"El promedio de calificaciones es: {promedio:.2f}")
    return promedio  # Retorna el valor (requisito técnico)


def buscar_estudiante_por_nombre():
    """Búsqueda lineal implementada MANUALMENTE (requisito obligatorio)."""
    print("\nBuscar Estudiante por Nombre")
    if not estudiantes:
        print("No hay estudiantes registrados.")
        return

    nombre_buscar = input("Ingrese el nombre a buscar: ").strip().lower()

    # Búsqueda lineal manual (recorremos la lista una por una)
    encontrado = False
    for est in estudiantes:
        if est["nombre"].lower() == nombre_buscar:
            print("Estudiante encontrado:")
            print(f"   Nombre: {est['nombre']}")
            print(f"   Edad: {est['edad']}")
            print(f"   Calificación: {est['calificacion']:.2f}")
            encontrado = True
            break  # Salimos una vez encontrado (búsqueda lineal)

    if not encontrado:
        print("Estudiante no encontrado.")


def buscar_mayor_menor_calificacion():
    """Encuentra mayor y menor calificación recorriendo la lista manualmente."""
    print("\nEstudiante con Mayor y Menor Calificación")
    if not estudiantes:
        print("No hay estudiantes registrados.")
        return

    # Inicializamos con el primer estudiante
    max_cal = estudiantes[0]["calificacion"]
    min_cal = estudiantes[0]["calificacion"]
    max_nombre = estudiantes[0]["nombre"]
    min_nombre = estudiantes[0]["nombre"]

    # Recorrido manual (algoritmo de búsqueda de máximo y mínimo)
    for est in estudiantes:
        if est["calificacion"] > max_cal:
            max_cal = est["calificacion"]
            max_nombre = est["nombre"]
        if est["calificacion"] < min_cal:
            min_cal = est["calificacion"]
            min_nombre = est["nombre"]

    print(f"Mayor calificación: {max_nombre} ({max_cal:.2f})")
    print(f"Menor calificación: {min_nombre} ({min_cal:.2f})")


def menu_principal():
    """Menú principal del sistema (bucle while)."""
    while True:
        print("SISTEMA DE GESTIÓN DE ESTUDIANTES")
        print("1. Agregar estudiante")
        print("2. Mostrar todos los estudiantes")
        print("3. Calcular el promedio de calificaciones")
        print("4. Buscar estudiante por nombre (búsqueda lineal)")
        print("5. Buscar estudiante con mayor y menor calificación")
        print("6. Salir del programa")
        print("=" * 55)

        opcion = input("Seleccione una opción (1-6): ").strip()

        if opcion == "1":
            agregar_estudiante()
        elif opcion == "2":
            mostrar_estudiantes()
        elif opcion == "3":
            calcular_promedio()
        elif opcion == "4":
            buscar_estudiante_por_nombre()
        elif opcion == "5":
            buscar_mayor_menor_calificacion()
        elif opcion == "6":
            print("\nSaliendo del programa. . .")
            break
        else:
            print("Opción inválida. Por favor ingrese un número del 1 al 6.")


if __name__ == "__main__":
    print("Bienvenido al Sistema de Gestión de Estudiantes")
    print("   Proyecto Final - Algoritmos y Programación")
    menu_principal()
