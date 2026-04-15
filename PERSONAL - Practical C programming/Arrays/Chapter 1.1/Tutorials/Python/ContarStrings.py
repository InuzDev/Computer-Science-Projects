def analyze_text(text):
    vowels = "aeiouAEIOU"
    vowel_count = 0
    letter_count = 0

    for character in text:
        if character in vowels:
            vowel_count += 1
        if character.isalpha():
            letter_count += 1

    percentage = (vowel_count / letter_count) * 100

    print("Characters:", len(text))
    print(f"Total vowels: {vowel_count}")
    print(f"Percentage: {percentage:.2f}%")


analyze_text("Hola Mundo")
