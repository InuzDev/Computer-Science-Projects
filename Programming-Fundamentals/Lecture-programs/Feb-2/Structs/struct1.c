#include <stdio.h>

#define MAX_ID 10
#define LENGHT_NAME 30
#define LENGHT_SURNAME 20

struct student {
   char id[MAX_ID];
   char name[LENGHT_NAME];
   char surname[LENGHT_SURNAME];
   int age;
   float index;
};

int main() {
   struct student est;

   printf("Estudiante: \n");
   printf("ID: ");
   // We need to learn how to get fgets.
   gets(est.id);

   printf("Name: ");
   gets(est.name);

   printf("Surname: ");
   gets(est.surname);

   printf("Age: ");
   scanf("%d", &est.age);

   printf("Index: ");
   scanf("%f", &est.index);

   printf("%-*s %-*s %-*s %s %s\n", MAX_ID, "Id", LENGHT_NAME, "Name", LENGHT_SURNAME, "Surname", "Age", "Index");
   printf("%-*s %-*s %-*s %4d %6.1f", MAX_ID, est.id, LENGHT_NAME, est.name, LENGHT_SURNAME, est.surname, est.age, est.index);

   return 0;
}
