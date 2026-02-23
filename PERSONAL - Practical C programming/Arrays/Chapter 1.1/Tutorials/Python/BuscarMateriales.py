Materiales = [["Cuadernos", 5], ["Bolígrafos", 7], ["Reglas", 3], ["Gomas", 4]]


def BuscarMaterial(materiales, busqueda):
    for sublista in materiales:
        if sublista[0] == busqueda:  # sublista[0] is the name
            print(f"Hay {sublista[1]} unidades de {sublista[0]}")
            return  # stop searching, we found it

    print(f"'{busqueda}' no existe. Se agregará con valor 0.")
    materiales.append([busqueda, 0])
    print("Lista actualizada:", materiales)


material = input("Digite un material: ")
BuscarMaterial(Materiales, material)
