def CombinarListas(letras, numeros):
    resultado = []

    for i in range(len(letras)):
        sublista = [letras[i], numeros[i]]
        resultado.append(sublista)

    return resultado


letras = ["A", "B", "C", "D", "E"]
num = [1, 23, 4, 12, 11]

print(CombinarListas(letras, num))
