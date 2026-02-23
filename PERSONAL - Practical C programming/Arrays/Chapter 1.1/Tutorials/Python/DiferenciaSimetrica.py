def DiferenciaSimetrica(lista1, lista2):
    resultado = []  # arreglo -> Un arreglo, es un tipo de dato que almacena x cantidad, de y tipo
    # Ejemplo: Arreglo resultado, almacena hasta la cantidad de informacion que le ingrese lista1 y lista2.

    # Check elements in lista1 that are NOT in lista2
    for elemento in lista1:  # lista = [123, 2, 45, ... , n]
        if elemento not in lista2:
            resultado.append(elemento)

    for elemento in lista2:
        if elemento not in lista1:
            resultado.append(elemento)

    resultado.sort()
    return resultado


list1 = [324, 23, 12, 58, 238, 398]
list2 = [324, 22, 11, 58, 234, 398]

print(DiferenciaSimetrica(list1, list2))
