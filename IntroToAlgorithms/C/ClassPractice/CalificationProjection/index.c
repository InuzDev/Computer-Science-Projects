/**
 * index.c
 *
 *
 * Carlos David Jorge Taveras ; 26/11/2025
 *
 * Programa modular el cual toma un texto de un archivo de texto y te dice:
 * la cantidad de palabras.
 * Cantidad d econsonantes y vocales sin repetición.
 * El caracter que más se repite y cuantas veces se repite en el texto.
 * Palabra que más se repite y cuantas veces se repite en el texto.
 * Palabra más grande y palabra más pequeña indicando el tamaño.
 * Cantidad de palabras por tamaño.
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_CHAR 1921
#define MAX_WORDS 400
#define MAX_WORD_LENGTH 50

int WordSeparator(char Text[], int size, char WordList[][MAX_WORD_LENGTH]);
void countUniqueVowelsConsonants(char Text[], int size, int VocalResult[]);
void CountBySize(char WordList[][MAX_WORD_LENGTH], int Quantity, int Count[]);
int MostShortWordIndex(char WordList[][MAX_WORD_LENGTH], int Quantity);
int MostLongWordIndex(char WordList[][MAX_WORD_LENGTH], int Quantity);
int MostRepeatedWord(char WordList[][MAX_WORD_LENGTH], int Quantity);
char MostRepeatedChar(char Text[], int size);
int MostRepeatedCharQuantity(char Text[], int size, char TargetCharacter);

int main()
{
   char Text[MAX_CHAR];
   char WordList[MAX_WORDS][MAX_WORD_LENGTH];
   char _MostRepeatedChar;

   int CharCount = 0;
   int VocalsAndConsonant[2];
   int ReadChar, WordQuantity, MaxQuantity, RepeatedIndex, LongIndex, ShortIndex;

   while (1)
   {
      ReadChar = getchar();

      if (ReadChar == EOF)
      {
         break;
      }

      if (CharCount < MAX_CHAR - 1)
      {
         Text[CharCount] = ReadChar;
         CharCount++;
      }
   }
   Text[CharCount] = '\0';
   countUniqueVowelsConsonants(Text, CharCount, VocalsAndConsonant);

   WordQuantity = WordSeparator(Text, CharCount, WordList);

   _MostRepeatedChar = MostRepeatedChar(Text, CharCount);
   MaxQuantity = MostRepeatedCharQuantity(Text, CharCount, _MostRepeatedChar);

   RepeatedIndex = MostRepeatedWord(WordList, WordQuantity);

   LongIndex = MostLongWordIndex(WordList, WordQuantity);
   ShortIndex = MostShortWordIndex(WordList, WordQuantity);

   int lengthCount[MAX_WORD_LENGTH + 1];
   CountBySize(WordList, WordQuantity, lengthCount);

   printf("\nCantidad de palabras: %d\n", WordQuantity);

   printf("Vocales unicas: %d, Consonantes unicas: %d\n", VocalsAndConsonant[0], VocalsAndConsonant[1]);

   printf("Caracter mas repetido: '%c' (%d veces)\n", _MostRepeatedChar, MaxQuantity);

   printf("Palabra mas repetida: %s\n", WordList[RepeatedIndex]);

   printf("Palabra mas larga: %s (caracteres %d)\n", WordList[LongIndex], (int)strlen(WordList[LongIndex]));
   printf("Palabra mas corta: %s (caracteres %d)\n", WordList[ShortIndex], (int)strlen(WordList[ShortIndex]));

   printf("\nCantidad de palabras por size:\n");
   int sizeIndex;
   for (sizeIndex = 1; sizeIndex <= MAX_WORD_LENGTH; sizeIndex++)
   {
      if (lengthCount[sizeIndex] > 0)
      {
         printf("Tama%co %d: %d palabras\n", 164, sizeIndex, lengthCount[sizeIndex]);
      }
   }

   return 0;
}

/**
 * funcion: WordSeparator
 * Objetivo: Leer el texto y extraer cada palabra
 * Argumentos: (char[]) Text, (char[][]) WordList, (int) size
 * Retorna: WordQuantity, que es la cantidad de palabras encontradas.
 */
int WordSeparator(char Text[], int size, char WordList[][MAX_WORD_LENGTH])
{
   int WordQuantity = 0;
   int LetterIndex = 0;

   for (int Index = 0; Index < size; Index++)
   {

      if (isalpha(Text[Index]))
      {
         if (LetterIndex < MAX_WORD_LENGTH - 1)
         {
            WordList[WordQuantity][LetterIndex] = tolower(Text[Index]);
            LetterIndex++;
         }
      }
      else
      {
         if (LetterIndex > 0)
         {
            WordList[WordQuantity][LetterIndex] = '\0';
            WordQuantity++;
            LetterIndex = 0;
         }
      }
   }

   if (LetterIndex > 0)
   {
      WordList[WordQuantity][LetterIndex] = '\0';
      WordQuantity++;
   }

   return WordQuantity;
}

/**
 * Funcion: countUniqueVowelsConsonants
 * Objetivo: Contar las vocales y consonantes del texto sin repetirlas.
 * Argumentos: (char[]) Text, (int) size, (int[]) VocalResult
 */
void countUniqueVowelsConsonants(char Text[], int size, int VocalResult[])
{
   int Vocals[5] = {0};
   int Consonants[21] = {0};
   char VocalsList[] = "aeiou";

   for (int index = 0; index < size; index++)
   {

      if (isalpha(Text[index]))
      {
         char Character = tolower(Text[index]);
         int found = 0;
         if (Character == -31)
         {
            Character = 'a';
         }
         if (Character == -23)
         {
            Character = 'e';
         }
         if (Character == -19)
         {
            Character = 'i';
         }
         if (Character == -13)
         {
            Character = 'o';
         }
         if (Character == -6)
         {
            Character = 'u';
         }
         if (Character == -15)
         {
            Character = 'n';
         }

         for (int indiceV = 0; indiceV < 5; indiceV++)
         {
            if (Character == VocalsList[indiceV])
            {
               Vocals[indiceV] = 1;
               found = 1;
               break;
            }
         }

         if (!found)
         {
            int ConsonantPosition = Character - 'a';
            if (ConsonantPosition >= 0 && ConsonantPosition < 26 &&
                Character != 'a' && Character != 'e' && Character != 'i' &&
                Character != 'o' && Character != 'u')
            {

               if (ConsonantPosition >= 0 && ConsonantPosition < 21)
                  Consonants[ConsonantPosition] = 1;
            }
         }
      }
   }

   int TotalVocals = 0;
   int TotalConsonants = 0;

   for (int index = 0; index < 5; index++)
      TotalVocals += Vocals[index];

   for (int index = 0; index < 21; index++)
      TotalConsonants += Consonants[index];

   VocalResult[0] = TotalVocals;
   VocalResult[1] = TotalConsonants;
}

/**
 * Funcion: MostRepeatedChar
 * Objetivo: Encontrar  la letra mas repetida
 * Argumentos: (char[]) Text, (int) size
 * Retorna: La letra mas repetida
 */
char MostRepeatedChar(char Text[], int size)
{
   int Frequency[256];
   char ActualChar;
   int MaxFrequency = 0;
   char FinalChar = '\0';

   for (int index = 0; index < 256; index++)
   {
      Frequency[index] = 0;
   }

   for (int index = 0; index < size; index++)
   {
      ActualChar = Text[index];

      if (ActualChar == ' ' || ActualChar == '\n' || ActualChar == '\t')
      {
         continue; /* ignorar espacios y saltos de línea */
      }

      Frequency[(unsigned char)ActualChar]++;

      if (Frequency[(unsigned char)ActualChar] > MaxFrequency)
      {
         MaxFrequency = Frequency[(unsigned char)ActualChar];
         FinalChar = ActualChar;
      }
   }

   return FinalChar;
}

/**
 * Funcion: MostRepeatedCharQuantity
 * Objetivo: Encontrar la cantidad de la letra mas repetida
 * Argumentos: (char[]) Text, (int) size, (char) targetCharacter
 * Retorna: La cantidad letra mas repetida
 */
int MostRepeatedCharQuantity(char Text[], int size, char targetCharacter)
{
   int index;
   int Counter = 0;

   for (index = 0; index < size; index++)
   {
      if (Text[index] == targetCharacter)
      {
         Counter++;
      }
   }

   return Counter;
}

/**
 * Funcion: MostRepeatedWord
 * Objetivo: Encontrar la palabra mas repetida
 * Argumentos: (char[][]) WordList, (int) Quantity
 * Retorna: La palabra mas repetida
 */
int MostRepeatedWord(char WordList[][MAX_WORD_LENGTH], int Quantity)
{
   int FoundWord = 0;
   int maximumRepetitions = 0;

   for (int index = 0; index < Quantity; index++)
   {
      int repeatCount = 1;

      for (int offsetIndex = index + 1; offsetIndex < Quantity; offsetIndex++)
      {
         if (strcmp(WordList[index], WordList[offsetIndex]) == 0)
         {
            repeatCount++;
         }
      }

      if (repeatCount > maximumRepetitions)
      {
         maximumRepetitions = repeatCount;
         FoundWord = index;
      }
   }

   return FoundWord;
}

/**
 * Funcion: MostLongWordIndex
 * Objetivo: Buscar la palabra mas larga del texto
 * Argumentos: (char[][]) WordList, (int) Quantity
 * Retorna: La palabra las larga del texto
 */
int MostLongWordIndex(char WordList[][MAX_WORD_LENGTH], int Quantity)
{
   int FoundWord = 0;
   int maxLength = strlen(WordList[0]);

   for (int index = 1; index < Quantity; index++)
   {
      int largoActual = strlen(WordList[index]);
      if (largoActual > maxLength)
      {
         maxLength = largoActual;
         FoundWord = index;
      }
   }

   return FoundWord;
}

/**
 * Funcion: MostShortWordIndex
 * Objetivo: Encontrar la palabra mas corta del texto
 * Argumentos: (char[][]) WordList, (int) Quantity
 * Retorna: La palabra mas corta del texto
 */
int MostShortWordIndex(char WordList[][MAX_WORD_LENGTH], int Quantity)
{
   int FoundWord = 0;
   int MinLongest = strlen(WordList[0]);

   for (int index = 1; index < Quantity; index++)
   {
      int largoActual = strlen(WordList[index]);
      if (largoActual < MinLongest)
      {
         MinLongest = largoActual;
         FoundWord = index;
      }
   }

   return FoundWord;
}

/**
 * Funcion: CountBySize
 * Objetivo: Contar el tamaño del texto
 * Argumentos: (char[][]) WordList, (int) quantity, (int[]) Count
 */
void CountBySize(char WordList[][MAX_WORD_LENGTH], int Quantity, int Count[])
{
   for (int index = 0; index < MAX_WORD_LENGTH + 1; index++)
      Count[index] = 0;

   for (int index = 0; index < Quantity; index++)
   {
      int wordLength = strlen(WordList[index]);
      if (wordLength == 0)
      {
         continue;
      }
      Count[wordLength]++;
   }
}