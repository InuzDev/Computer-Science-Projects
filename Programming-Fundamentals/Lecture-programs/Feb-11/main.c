#include <conio.c>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CT BLACK
#define CF BLUE
#define CTS BLACK
#define CFS GREEN
#define CET BLACK
#define CEF BLUE

#define ARRIBA 72
#define ABAJO 80
#define ESC 27
#define ENTER 13

#define MAXID 6
#define FNAME 21
#define LNAME 16

#define POSXINI 5
#define POSYINI 5

typedef struct {
   int dd, mm, yyyy;
} FECHA;

typedef struct {
   char empl_id[MAXID], nombre[FNAME], apellido[LNAME];
   FECHA fecnacim, feccontract;
   float salario, comision;
   char jefe_id[MAXID];
} EMPLEADO;

void clear_buffer();
int getempls(EMPLEADO **empls);
int empleado_existe(EMPLEADO *empls, int count, char *id);
EMPLEADO *getemp(void);
void showempl(EMPLEADO *empls, int num_empls, int posx, int posy);

int calcular_edad(FECHA fecnacim);
int calcular_antiguedad(FECHA feccontract);
void setcolor(int ct, int cf);
void colordefault();
void showopcxy_empleados(EMPLEADO *empls, int n, int py, int px, int opcsel);
int movimiento_empleados(EMPLEADO *empls, int n, int py, int px, int opcsel);

int main() {
   EMPLEADO *empls;

   int cantempls = getempls(&empls);
   showempl(empls, cantempls, POSXINI, POSYINI);

   free(empls);

   return 0;
}

EMPLEADO *getemp(void) {
   EMPLEADO *empl = (EMPLEADO *)malloc(sizeof(EMPLEADO));

   if (empl == NULL) {
      printf("Alojamiento de memoria fallido\n");
      return NULL;
   }

   clear_buffer();
   printf("Ingrese el nombre del empleado: ");
   scanf("%20s", empl->nombre);

   clear_buffer();
   printf("Ingrese el apellido del empleado: ");
   scanf("%15s", empl->apellido);

   clear_buffer();
   printf("Ingrese el ID del jefe [0000]: ");
   scanf("%5s", empl->jefe_id);

   clear_buffer();
   printf("Ingrese el salario del empleado: ");
   scanf("%f", &empl->salario);

   clear_buffer();
   printf("Ingrese la comision del empleado: ");
   scanf("%f", &empl->comision);

   clear_buffer();
   printf("Ingrese fecha de nacimiento (dd/mm/yyyy): ");
   scanf("%d/%d/%d", &empl->fecnacim.dd, &empl->fecnacim.mm, &empl->fecnacim.yyyy);

   clear_buffer();
   printf("Ingrese fecha de contrato (dd/mm/yyyy): ");
   scanf("%d/%d/%d", &empl->feccontract.dd, &empl->feccontract.mm, &empl->feccontract.yyyy);

   return empl;
}

int getempls(EMPLEADO **empls) {
   int emplsCount = 0;
   char UserInput;
   char temp_id[MAXID];
   EMPLEADO *temp = NULL;

   while (1) {
      printf("Quieres capturar un empleado? [S/N]:\n");
      scanf("%c", &UserInput);

      UserInput = tolower(UserInput);

      if (UserInput == 'n' || UserInput == 's') {
         break;
      }
      printf("Por favor. Entrar una letra valida\n\n");
   }

   *empls = NULL;

   if (UserInput == 's') {
      while (UserInput == 's') {
         clear_buffer();
         printf("Ingrese el ID del empleado [0000]: ");
         scanf("%5s", temp_id);

         if (empleado_existe(*empls, emplsCount, temp_id)) {
            printf("ERROR: El empleado con ID %s ya existe!\n", temp_id);

            printf("\nQuieres agregar otro empleado? [S/N]: ");
            clear_buffer();
            scanf("%c", &UserInput);
            UserInput = tolower(UserInput);
            continue;
         }

         temp = (EMPLEADO *)realloc(*empls, (emplsCount + 1) * sizeof(EMPLEADO));

         if (temp == NULL) {
            printf("Alojamiento de memoria fallido\n");
            free(*empls);
            *empls = NULL;
            return emplsCount;
         }

         *empls = temp;

         strcpy((*empls)[emplsCount].empl_id, temp_id);

         EMPLEADO *empl = getemp();
         if (empl != NULL) {
            strcpy((*empls)[emplsCount].nombre, empl->nombre);
            strcpy((*empls)[emplsCount].apellido, empl->apellido);
            (*empls)[emplsCount].fecnacim = empl->fecnacim;
            (*empls)[emplsCount].feccontract = empl->feccontract;
            (*empls)[emplsCount].salario = empl->salario;
            (*empls)[emplsCount].comision = empl->comision;
            strcpy((*empls)[emplsCount].jefe_id, empl->jefe_id);

            if (emplsCount > 0) {
               if (!empleado_existe(*empls, emplsCount, empl->jefe_id)) {
                  printf("ADVERTENCIA: El jefe con ID %s no existe en el sistema!\n", empl->jefe_id);
               }
            }

            free(empl);
            emplsCount++;
         }

         printf("\nQuieres agregar otro empleado? [S/N]: ");
         clear_buffer();
         scanf("%c", &UserInput);
         UserInput = tolower(UserInput);
      }
   }
   return emplsCount;
}

int empleado_existe(EMPLEADO *empls, int count, char *id) {
   for (int ind = 0; ind < count; ind++) {
      if (strcmp(empls[ind].empl_id, id) == 0) {
         return 1;
      }
   }
   return 0;
}

void clear_buffer() {
   int Character;
   while ((Character = getchar()) != '\n' && Character != EOF)
      ;
}

int calcular_edad(FECHA fecnacim) {
   time_t _time = time(NULL);
   struct tm *fecha = localtime(&_time);

   int edad = (fecha->tm_year + 1900) - fecnacim.yyyy;

   if (fecnacim.mm > (fecha->tm_mon + 1) ||
       (fecnacim.mm == (fecha->tm_mon + 1) && fecnacim.dd > fecha->tm_mday)) {
      edad--;
   }

   return edad;
}

int calcular_antiguedad(FECHA feccontract) {
   time_t _time = time(NULL);
   struct tm *fecha = localtime(&_time);

   int antiguedad = (fecha->tm_year + 1900) - feccontract.yyyy;

   if (feccontract.mm > (fecha->tm_mon + 1) ||
       (feccontract.mm == (fecha->tm_mon + 1) && feccontract.dd > fecha->tm_mday)) {
      antiguedad--;
   }

   return antiguedad;
}

void showempl(EMPLEADO *empls, int num_empls, int posx, int posy) {
   system("cls");
   movimiento_empleados(empls, num_empls, posy, posx, 0);
}

void showopcxy_empleados(EMPLEADO *empls, int numEmp, int py, int px, int opcsel) {
   int ind;
   char nombre_completo[FNAME + LNAME + 1];
   char nombre_jefe[FNAME + LNAME + 1];

   gotoxy(px, py);
   setcolor(CET, CEF);
   printf("%-6s %-35s %-5s %-11s %-10s %-25s",
          "Id", "Nombre", "Edad", "Antiguedad", "Salario", "Jefe");

   for (ind = 0; ind < numEmp; ind++) {
      setcolor(CT, CF);

      if (opcsel == ind) {
         setcolor(CTS, CFS);
      }

      gotoxy(px, py + ind + 1);

      sprintf(nombre_completo, "%s %s", empls[ind].nombre, empls[ind].apellido);

      strcpy(nombre_jefe, "N/A");
      for (int j = 0; j < numEmp; j++) {
         if (strcmp(empls[j].empl_id, empls[ind].jefe_id) == 0) {
            sprintf(nombre_jefe, "%s %s", empls[j].nombre, empls[j].apellido);
            break;
         }
      }

      printf("%-6s %-35s %-5d %-11d %-10.2f %-25s",
             empls[ind].empl_id,
             nombre_completo,
             calcular_edad(empls[ind].fecnacim),
             calcular_antiguedad(empls[ind].feccontract),
             empls[ind].salario,
             nombre_jefe);
   }

   gotoxy(px, py + numEmp + 1);
   setcolor(CET, CEF);
   printf("TOTAL: %d", numEmp);

   colordefault();
}

int movimiento_empleados(EMPLEADO *empls, int numempl, int py, int px, int opcsel) {
   char tecla;

   _setcursortype(0);

   do {
      showopcxy_empleados(empls, numempl, px, py, opcsel);

      do {
         tecla = getch();
      } while (tecla != ESC && tecla != ENTER && tecla != ARRIBA && tecla != ABAJO);

      if (tecla != ESC) {
         if (tecla == ARRIBA) {
            if (opcsel != 0)
               opcsel--;
            else
               opcsel = numempl - 1;
         }

         if (tecla == ABAJO) {
            if (opcsel < numempl - 1)
               opcsel++;
            else
               opcsel = 0;
         }
      }

   } while (tecla != ESC && tecla != ENTER);

   _setcursortype(100);

   return opcsel;
}

void setcolor(int ct, int cf) {
   textbackground(cf);
   textcolor(ct);
}

void colordefault() {
   setcolor(LIGHTGRAY, BLACK);
}
