def ConvertTemps(InputTemp):

    Celcius = (InputTemp - 32) * 5 / 9
    Fahrenheit = (InputTemp * 9 / 5) + 32

    print("Conversion de celcius a Fahrenheit: ", Fahrenheit)
    print("Conversion de Fahrenheit a celcius: ", Celcius)


ConvertTemps(0)
