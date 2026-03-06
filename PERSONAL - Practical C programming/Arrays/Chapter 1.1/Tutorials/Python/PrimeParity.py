def count_factors(number):
    count = 0
    for index in range(1, number + 1):
        if number % index == 0:
            count += 1
    return count


def prime_parity_average(start, end):
    numbers = []
    for number in range(start, end + 1):
        if count_factors(number) % 2 == 0:
            numbers.append(number)

    return sum(numbers) / len(numbers)


start = int(input("Enter the start of the range: "))
end = int(input("Enter the end of the range: "))

print(prime_parity_average(start, end))
