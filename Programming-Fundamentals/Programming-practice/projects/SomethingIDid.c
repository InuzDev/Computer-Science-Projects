#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGHT 32
#define MAX_HOBBIES_LENGHT 128
#define MAX_CAREER_LENGHT 64
#define MAX_ORIENTATION_LENGHT 12

struct Presentation {
   char name[MAX_NAME_LENGHT];
   int age;
   char hobbies[MAX_HOBBIES_LENGHT];
   char sex; // Male or Female [M/F] as it is only a character.
   char career[MAX_CAREER_LENGHT];
   char orientation[MAX_ORIENTATION_LENGHT];
};

int Checkgender(char sex);

int main() {
   struct Presentation prstn;

   printf("Ingrese su presentaci%cn\n", 162);

   printf("Nombre: ");
   fgets(prstn.name, MAX_NAME_LENGHT, stdin);
   prstn.name[strcspn(prstn.name, "\n")] = 0;

   printf("Edad: ");
   scanf("%d", &prstn.age);
   getchar();

   printf("Hobbies: ");
   fgets(prstn.hobbies, MAX_HOBBIES_LENGHT, stdin);
   prstn.hobbies[strcspn(prstn.hobbies, "\n")] = 0;

   printf("Sexo: ");
   scanf("%c", &prstn.sex);
   getchar();
   int GenderCheck = Checkgender(prstn.sex);
   if (GenderCheck) {
      printf("Cerrando programa - Error LGBT-404");
      return 0;
   }

   printf("Carrera: ");
   fgets(prstn.career, MAX_CAREER_LENGHT, stdin);
   prstn.career[strcspn(prstn.career, "\n")] = 0;

   printf("Orientaci%cn: ", 162);
   fgets(prstn.orientation, MAX_ORIENTATION_LENGHT, stdin);
   prstn.orientation[strcspn(prstn.orientation, "\n")] = 0;

   printf("\nPresentaci%cn!\n", 162);
   printf("-----------------\n");
   printf("Nombre: %s\n", prstn.name);
   printf("Edad: %d\n", prstn.age);
   printf("Hobbies: %s\n", prstn.hobbies);
   printf("Sexo: %c\n", prstn.sex);
   printf("Carrera: %s\n", prstn.career);
   printf("Orientaci%cn: %s\n", 162, prstn.orientation);
   printf("-----------------\n");

   return 0;
}

int Checkgender(char sex) {
   sex = toupper(sex);
   if (sex != 'M' || sex != 'F') {
      return 0;
   } else {
      return 1;
   }
   return 0;
}
