num = int(input("Digite un valor: "))


def sum_of_divisors(num):
    total = 0
    for indice in range(1, num):
        if num % indice == 0:
            total += indice  # total = total + indice
    return total  # sum_of_divisors(num) = Total


def classify_number(num):
    total_divisores = sum_of_divisors(num)

    print(f"Number: {num}")
    print(f"Sum of divisors: {total_divisores}")

    if total_divisores == num:
        print("The number is PERFECT ✓")
    elif total_divisores < num:
        print("The number is DEFICIENT")
    else:
        print("The number is ABUNDANT")


classify_number(num)
