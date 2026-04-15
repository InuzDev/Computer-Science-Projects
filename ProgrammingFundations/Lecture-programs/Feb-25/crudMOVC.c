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

#define SELECTED_TEXTCOLOR BLACK
#define SELECTED_BGCOLOR BLUE
#define TEXT_COLOR LIGHTGRAY
#define BG_COLOR BLACK

#define OPT_LENGTH 81

#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75
#define ESC 27
#define ENTER 13
#define BACKSPACE 8

#define MAXNUM_LEN 10
#define MAXID 6
#define FNAME 21
#define LNAME 16

#define POSXINI 1
#define POSYINI 1

#define TABLE_XINIT 1
#define TABLE_YINIT 2

#define FORM_Y 4

typedef struct {
   int dd, mm, yyyy;
} FECHA;

typedef struct {
   char empl_id[MAXID], nombre[FNAME],
       apellido[LNAME];
   FECHA fecnacim, feccontract;
   float salario, comision;
   char jefe_id[MAXID];
} EMPLEADO;

int validate_date(int day, int month, int year);
void ModifyEmployee();
void clear_buffer();
int EditForm(EMPLEADO *empl, int isNew);
void ListFromFile();
int getLastId();
int getempls(EMPLEADO **empls);
int empleado_existe(EMPLEADO *empls, int count, char *id);
EMPLEADO *getemp(void);
FECHA captureDate(char *date_str, int field_length, int pos_x, int pos_y);
void CaptureInteger(char *digit_str, int *result_value, int max_digits, int pos_x, int pos_y);
void captureAlphanumeric(char *result_str, int max_length, int Xpos, int Ypos, int (*validator)(char));
void showempl(EMPLEADO *empls, int num_empls, int posx, int posy);
void show_form(EMPLEADO *empls, int isSelected);
void show_header(char menu[][OPT_LENGTH], int Options, int Xpos, int Ypos, int isSelected);
void show_field(char *field_str, int cursor_position, int max_length, int pos_x, int pos_y);
int calcular_edad(FECHA fecnacim);
int calcular_antiguedad(FECHA feccontract);
void setcolor(int ct, int cf);
void colordefault();
void showopcxy_empleados(EMPLEADO *empls, int n, int py, int px, int opcsel);
int movimiento_empleados(EMPLEADO *empls, int n, int py, int px, int opcsel);
int validate_alpha_char(char _char);
int validate_alphanumeric_char(char _char);
void ingresar_empleado();

int main() {
   char menu[][OPT_LENGTH] = {"Ingresar", "Modificar", "Listar", "Salvar", "Salir"};
   int isSelected = 0;
   int key;

   do {
      system("cls");
      show_header(menu, 5, 1, 1, isSelected);

      key = getch();
      if (key == 0 || key == 224)
         key = getch();

      if (key == LEFT && isSelected > 0)
         isSelected--;
      if (key == RIGHT && isSelected < 4)
         isSelected++;

      if (key == ENTER) {
         switch (isSelected) {
         case 0: {
            EMPLEADO nuevo;
            memset(&nuevo, 0, sizeof(EMPLEADO));

            int next_id = getLastId();
            sprintf(nuevo.empl_id, "%05d", next_id);
            if (EditForm(&nuevo, 1)) {
               FILE *file = fopen("employees.dat", "ab");
               if (file) {
                  fwrite(&nuevo, sizeof(EMPLEADO), 1, file);
                  fclose(file);
               }
            }
            break;
         }
         case 1:
            ModifyEmployee();
            break;
         case 2:
            ListFromFile();
            break;
         case 4:
            key = ESC;
            break;
         }
      }
   } while (key != ESC);

   return 0;
}
/*
 * Funcion: getLastId
 * Objetivo: Obtener el ID maximo para generar el siguiente
 * Retorna: El maximo ID.
 */
int getLastId() {
   FILE *file = fopen("employees.dat", "rb");
   if (file == NULL) {
      return 1;
   }

   EMPLEADO temp;
   int max = 0;
   while (fread(&temp, sizeof(EMPLEADO), 1, file)) {
      int current = atoi(temp.empl_id);
      if (current > max) {
         max = current;
      }
   }
   fclose(file);
   return max + 1;
}

void ListFromFile() {
   FILE *file = fopen("employees.dat", "rb");
   if (!file)
      return;

   fseek(file, 0, SEEK_END);
   int count = ftell(file) / sizeof(EMPLEADO);
   rewind(file);

   EMPLEADO *temp_list = (EMPLEADO *)malloc(count * sizeof(EMPLEADO));
   fread(temp_list, sizeof(EMPLEADO), count, file);
   fclose(file);

   system("cls");
   movimiento_empleados(temp_list, count, TABLE_YINIT, TABLE_XINIT, 0);

   free(temp_list);
}

void ingresar_empleado() {
   EMPLEADO nuevo;
   int max_id = getLastId();
   sprintf(nuevo.empl_id, "%05d", max_id + 1);
}

void ModifyEmployee() {
   FILE *file = fopen("employees.dat", "rb+");
   if (!file)
      return;

   fseek(file, 0, SEEK_END);
   int count = ftell(file) / sizeof(EMPLEADO);
   rewind(file);

   EMPLEADO *lista = (EMPLEADO *)malloc(count * sizeof(EMPLEADO));
   fread(lista, sizeof(EMPLEADO), count, file);

   system("cls");
   int selected_index = movimiento_empleados(lista, count, TABLE_YINIT, TABLE_XINIT, 0);

   EMPLEADO editando = lista[selected_index];

   if (EditForm(&editando, 0)) {
      fseek(file, selected_index * sizeof(EMPLEADO), SEEK_SET);
      fwrite(&editando, sizeof(EMPLEADO), 1, file);
   }

   free(lista);
   fclose(file);
}

void show_header(char menu[][OPT_LENGTH], int Options, int Xpos, int Ypos, int isSelected) {
   gotoxy(Xpos, Ypos);
   setcolor(CT, CF);
   printf("Lista de empleados");

   int col_offset = strlen("Lista de empleados") + 2;

   for (int index = 0; index < Options; index++) {
      gotoxy(Xpos + col_offset, Ypos);

      if (isSelected == index)
         setcolor(CTS, CFS);
      else
         setcolor(CT, CF);

      printf("%s", menu[index]);
      col_offset += strlen(menu[index]) + 1;
      setcolor(CT, CF);
      printf("%c", ' ');
   }

   colordefault();
}

void captureAlphanumeric(char *result_str, int max_length, int Xpos, int Ypos,
                         int (*validator)(char)) {
   int cursor_index = 0;
   char pressed_key;

   _setcursortype(100);

   do {
      show_field(result_str, cursor_index, max_length, Xpos, Ypos);

      fflush(stdin);
      do {
         pressed_key = getch();
      } while (!validator(pressed_key) &&
               pressed_key != ENTER &&
               pressed_key != ESC &&
               pressed_key != LEFT &&
               pressed_key != BACKSPACE &&
               pressed_key != RIGHT);

      if (pressed_key == RIGHT) {
         if (cursor_index < max_length - 1)
            cursor_index++;
      } else if (pressed_key == LEFT) {
         if (cursor_index > 0)
            cursor_index--;
      } else if (pressed_key != ENTER && pressed_key != ESC) {
         if (pressed_key == BACKSPACE) {
            if (cursor_index) {
               cursor_index--;
               strcpy(result_str + cursor_index, result_str + cursor_index + 1);
            }
         } else {
            if (cursor_index < max_length) {
               *(result_str + cursor_index) = pressed_key;
               cursor_index++;
            }
         }
      }
   } while (pressed_key != ENTER && pressed_key != ESC);

   if (pressed_key != ESC)
      *(result_str + cursor_index) = '\0';
}

int EditForm(EMPLEADO *empl, int isNew) {
   int field = 1;
   int key, val;
   char temp_str[11];
   char temp_num[MAXNUM_LEN];
   int saved = 0;

   do {
      system("cls");
      gotoxy(1, 1);
      if (isNew)
         printf("MODO: Ingresar nuevo empleado");
      else
         printf("MODO: MODIFICAR (ID: %s)", empl->empl_id);

      show_form(empl, field);

      key = getch();
      if (key == 0 || key == 224)
         key = getch();

      if (key == DOWN && field < 8)
         field++;
      if (key == UP && field > 0)
         field--;

      if (key == 60) {
         saved = 1;
         break;
      }

      if (key == ENTER) {
         switch (field) {
         case 1:
            captureAlphanumeric(empl->nombre, FNAME - 1, POSXINI + 24, FORM_Y + 1, validate_alpha_char);
            break;
         case 2:
            captureAlphanumeric(empl->apellido, LNAME - 1, POSXINI + 24, FORM_Y + 2, validate_alpha_char);
            break;
         case 3:
            sprintf(temp_str, "%02d/%02d/%04d", empl->fecnacim.dd, empl->fecnacim.mm, empl->fecnacim.yyyy);
            empl->fecnacim = captureDate(temp_str, 10, POSXINI + 24, FORM_Y + 3);
            break;
         case 4:
            sprintf(temp_str, "%02d/%02d/%04d", empl->feccontract.dd, empl->feccontract.mm, empl->feccontract.yyyy);
            empl->feccontract = captureDate(temp_str, 10, POSXINI + 24, FORM_Y + 4);
            break;
         case 5:
            sprintf(temp_num, "%d", (int)empl->salario);
            CaptureInteger(temp_num, &val, MAXNUM_LEN - 1, POSXINI + 24, FORM_Y + 5);
            empl->salario = (float)val;
            break;
         case 6:
            sprintf(temp_num, "%d", (int)empl->comision);
            CaptureInteger(temp_num, &val, MAXNUM_LEN - 1, POSXINI + 24, FORM_Y + 6);
            empl->comision = (float)val;
            break;
         case 7:
            captureAlphanumeric(empl->jefe_id, MAXID - 1, POSXINI + 24, FORM_Y + 7, validate_alphanumeric_char);
            break;
         case 8:
            saved = 1;
            key = ESC;
            break;
         }
      }
   } while (key != ESC);

   return saved;
}

int validate_date(int day, int month, int year) {
   if (month >= 1 && month <= 12) {
      if (month == 2) {
         if (day >= 1 && day <= 28) {
            return 1;
         } else if (day == 29 && (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))) {
            return 1;
         }
      } else if (month == 4 || month == 6 || month == 9 || month == 11) {
         if (day >= 1 && day <= 30) {
            return 1;
         }
      } else {
         if (day >= 1 && day <= 31) {
            return 1;
         }
      }
   }

   return 0;
}

void show_form(EMPLEADO *empl, int isSelected) {
   // 1. Define the correct labels from the PDF
   char *labels[] = {
       "ID:",
       "Nombre:",
       "Apellido:",
       "Fecha nacim.:",
       "Fecha entrada:",
       "Salario:",
       "Comision:",
       "ID Jefe:",
       "SALVAR"};

   system("cls"); // Clear screen for the form view [cite: 11]
   gotoxy(1, FORM_Y - 2);
   printf("--- INGRESANDO / MODIFICANDO EMPLEADO ---");

   for (int i = 0; i < 9; i++) {
      gotoxy(5, FORM_Y + i);

      // 2. Handle highlighting
      if (isSelected == i)
         setcolor(SELECTED_TEXTCOLOR, SELECTED_BGCOLOR);
      else
         setcolor(TEXT_COLOR, BG_COLOR);

      printf("%-18s", labels[i]);

      // 3. Print the actual employee data [cite: 5, 12]
      // Move cursor to the right of the label
      gotoxy(25, FORM_Y + i);

      switch (i) {
      case 0:
         printf("%s", empl->empl_id);
         break;
      case 1:
         printf("%s", empl->nombre);
         break;
      case 2:
         printf("%s", empl->apellido);
         break;
      case 3:
         printf("%02d/%02d/%04d", empl->fecnacim.dd, empl->fecnacim.mm, empl->fecnacim.yyyy);
         break;
      case 4:
         printf("%02d/%02d/%04d", empl->feccontract.dd, empl->feccontract.mm, empl->feccontract.yyyy);
         break;
      case 5:
         printf("%.2f", empl->salario);
         break;
      case 6:
         printf("%.2f", empl->comision);
         break;
      case 7:
         printf("%s", empl->jefe_id);
         break;
      case 8:
         if (isSelected == 8)
            printf("<< PRESIONE ENTER PARA GUARDAR >>");
         break;
      }
      colordefault();
   }

   // 4. Update the help footer based on the PDF requirements [cite: 34, 41]
   gotoxy(1, FORM_Y + 11);
   setcolor(TEXT_COLOR, BG_COLOR);
   printf("Movimientos: %c%c | ENTER: Editar | ESC: Salir sin Salvar", 24, 25);
   colordefault();
}
FECHA captureDate(char *date_str, int field_length, int pos_x, int pos_y) {
   int cursor_index = 0;
   char pressed_key;
   FECHA result_date;

   _setcursortype(100);

   do {
      *(date_str + 2) = '/';
      *(date_str + 5) = '/';
      show_field(date_str, cursor_index, field_length, pos_x, pos_y);

      fflush(stdin);
      do {
         pressed_key = getch();
      } while ((pressed_key < '0' || pressed_key > '9') && pressed_key != ENTER && pressed_key != ESC &&
               pressed_key != LEFT && pressed_key != BACKSPACE && pressed_key != RIGHT);

      if (pressed_key == RIGHT) {
         if (cursor_index < field_length - 1) {
            cursor_index++;
            if (cursor_index == 2 || cursor_index == 5)
               cursor_index++;
         }
      } else if (pressed_key == LEFT) {
         if (cursor_index > 0) {
            cursor_index--;
            if (cursor_index == 2 || cursor_index == 5)
               cursor_index--;
         }
      } else if (pressed_key != ENTER && pressed_key != ESC) {
         if (pressed_key == BACKSPACE) {
            if (cursor_index) {
               cursor_index--;
               if (cursor_index == 2 || cursor_index == 5)
                  cursor_index--;
               strcpy(date_str + cursor_index, date_str + cursor_index + 1);
            }
         } else {
            if (cursor_index != 2 && cursor_index != 5 && cursor_index < field_length) {
               *(date_str + cursor_index) = pressed_key;
               cursor_index++;

               if (cursor_index == 2 || cursor_index == 5)
                  cursor_index++;
            }
         }
      }

      if (pressed_key == ENTER) {
         *(date_str + cursor_index) = '\0';

         char day_str[3] = {date_str[0], date_str[1], '\0'};
         char month_str[3] = {date_str[3], date_str[4], '\0'};
         char year_str[5] = {date_str[6], date_str[7], date_str[8], date_str[9], '\0'};

         result_date.dd = atoi(day_str);
         result_date.mm = atoi(month_str);
         result_date.yyyy = atoi(year_str);

         if (validate_date(result_date.dd, result_date.mm, result_date.yyyy)) {
            gotoxy(pos_x, pos_y + 2);
            setcolor(GREEN, BLACK);
            printf("Fecha valida!%20c", ' ');
            colordefault();
            break;
         } else {
            gotoxy(pos_x, pos_y + 2);
            setcolor(RED, BLACK);
            printf("Fecha invalida! Intente de nuevo.%20c", ' ');
            colordefault();
            memset(date_str, 0, field_length);
            cursor_index = 0;
         }
      }
   } while (pressed_key != ESC);

   return result_date;
}

void show_field(char *field_str, int cursor_position, int max_length, int pos_x, int pos_y) {
   setcolor(TEXT_COLOR, BG_COLOR);

   for (int index = 0; index < max_length; index++) {
      gotoxy(pos_x + index + 1, pos_y);
      printf("%c", *(field_str + index) ? *(field_str + index) : ' ');
   }

   colordefault();
   gotoxy(pos_x + cursor_position + 1, pos_y);
}

void CaptureInteger(char *digit_str, int *result_value, int max_digits, int pos_x, int pos_y) {
   int cursor_index = 0;
   char pressed_key;

   _setcursortype(100);

   do {
      show_field(digit_str, cursor_index, max_digits, pos_x, pos_y);

      fflush(stdin);
      do {
         pressed_key = getch();
      } while ((pressed_key < '0' || pressed_key > '9') &&
               pressed_key != ENTER &&
               pressed_key != ESC &&
               pressed_key != LEFT &&
               pressed_key != BACKSPACE &&
               pressed_key != RIGHT);

      if (pressed_key == RIGHT) {
         if (cursor_index < max_digits - 1)
            cursor_index++;
      } else if (pressed_key == LEFT) {
         if (cursor_index > 0)
            cursor_index--;
      } else if (pressed_key != ENTER && pressed_key != ESC) {
         if (pressed_key == BACKSPACE) {
            if (cursor_index) {
               cursor_index--;
               strcpy(digit_str + cursor_index, digit_str + cursor_index + 1);
            }
         } else {
            if (cursor_index < max_digits) {
               *(digit_str + cursor_index) = pressed_key;
               cursor_index++;
            }
         }
      }
   } while (pressed_key != ENTER && pressed_key != ESC);

   if (pressed_key != ESC) {
      *(digit_str + cursor_index) = '\0';
      *result_value = atoi(digit_str);
   }
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
      } while (tecla != ESC && tecla != ENTER && tecla != UP && tecla != DOWN);

      if (tecla != ESC) {
         if (tecla == UP) {
            if (opcsel != 0)
               opcsel--;
            else
               opcsel = numempl - 1;
         }

         if (tecla == DOWN) {
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

int validate_alpha_char(char _char) {
   return (isalpha(_char) || _char == ' ' || _char == '-');
}

int validate_alphanumeric_char(char _char) {
   return (isalpha(_char) || isdigit(_char) || _char == ' ' || _char == '-');
}
