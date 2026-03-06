#include <conio.c>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIELD_ID 0
#define FIELD_EMAIL 1
#define FIELD_NAME 2
#define FIELD_SURNAME 3
#define FIELD_PWD 4
#define FIELD_SAVE 5
#define FIELD_COUNT 6

#define ID_MAX_LENGTH 9
#define MAX_EMAIL_LENGTH 60
#define MAX_NAME_LENGTH 25
#define MAX_SURNAME_LENGTH 20
#define MAX_PWD_LENGTH 20

#define OPT_LENGTH 81
#define MAX_OPTIONS 5

#define INIT_XPOS 1
#define INIT_YPOS 1
#define TABLE_XPOS 1
#define TABLE_YPOS 2
#define FORM_YPOS 4

#define ARROW_UP 24
#define ARROW_DOWN 25

#define RIGHT 77
#define LEFT 75
#define UP 72
#define DOWN 80
#define ESC 27
#define ENTER 13
#define BACKSPACE 8
#define F2 60

#define ID_BASE 10000000

#define SELECTED_TEXTCOLOR BLACK
#define SELECTED_BGCOLOR GREEN
#define TEXT_COLOR LIGHTGRAY
#define BG_COLOR BLACK

typedef struct {
   char account_id[ID_MAX_LENGTH + 1];
   char email[MAX_EMAIL_LENGTH + 1];
   char name[MAX_NAME_LENGTH + 1];
   char surname[MAX_SURNAME_LENGTH + 1];
   char pwd[MAX_PWD_LENGTH + 1];
} ACCOUNTDB;

typedef struct {
   ACCOUNTDB *records;
   int count;
} ACCOUNTLIST;

int getLastId(char *dbFilename);
ACCOUNTLIST loadAccounts(char *dbFilename);
void insertAccount(char *dbFilename);
void deleteAccount(char *dbFilename);
void modifyAccount(char *dbFilename);
void scriptLogins(char *dbFilename, char *sqlFilename);
int EditForm(ACCOUNTDB *account, int isNew);
void show_form(ACCOUNTDB *account, int activeField, char *errorMsg);
void showOptions_Account(ACCOUNTLIST list, int selectedIndex, int Xpos, int Ypos);
int MenuMovement(ACCOUNTLIST list, int Xpos, int Ypos, int selectedIndex);
void show_header(char menu[][OPT_LENGTH], int optCount, int Xpos, int Ypos, int isSelected);
void show_field(char *field_str, int cursor_position, int max_length, int pos_x, int pos_y);
void CaptureAlphanumeric(char *result_str, int max_length, int Xpos, int Ypos, int (*validator)(char));
void setColor(int TextColor, int BackgroundColor);
void setDefaultColor();
int validate_email_format(char *email);
int validate_email_char(char inputChar);
int validate_alpha_char(char inputChar);
int validate_alphanumeric_char(char inputChar);

int main() {
   char studentId[ID_MAX_LENGTH + 1];
   char dbFilename[30];
   char sqlFilename[30];

   memset(studentId, 0, sizeof(studentId));

   system("cls");
   gotoxy(INIT_XPOS, INIT_YPOS);
   setColor(TEXT_COLOR, BG_COLOR);
   printf("Ingrese su ID de usuario: ");
   CaptureAlphanumeric(studentId, ID_MAX_LENGTH,
                       INIT_XPOS + 26, INIT_YPOS,
                       validate_alphanumeric_char);

   sprintf(dbFilename, "E5-%s.dat", studentId);
   sprintf(sqlFilename, "E5-%s.sql", studentId);

   char menu[MAX_OPTIONS][OPT_LENGTH] = {
       "INSERTAR",
       "ELIMINAR",
       "MODIFICAR",
       "SCRIPT LOGINS",
       "SALIR"};

   int pressedKey = 0;
   int isSelected = 0;

   do {
      system("cls");
      show_header(menu, MAX_OPTIONS, INIT_XPOS, INIT_YPOS, isSelected);

      pressedKey = getch();
      if (pressedKey == 0 || pressedKey == 224)
         pressedKey = getch();

      if (pressedKey == LEFT && isSelected > 0)
         isSelected--;
      else if (pressedKey == RIGHT && isSelected < MAX_OPTIONS - 1)
         isSelected++;

      if (pressedKey == ENTER) {
         switch (isSelected) {
         case 0:
            insertAccount(dbFilename);
            break;
         case 1:
            deleteAccount(dbFilename);
            break;
         case 2:
            modifyAccount(dbFilename);
            break;
         case 3:
            scriptLogins(dbFilename, sqlFilename);
            break;
         case 4:
            pressedKey = ESC;
            break;
         }
      }

   } while (pressedKey != ESC);

   return 0;
}

/*
 * Funcion : getLastId
 * Objetivo: Escanear la base de datos binaria del archivo hasta encontrar el ID maximo.
 *            Si no hay IDs, se genera uno con el ID base.
 * Retorna  : Retorna el siguiente ID maximo
 */
int getLastId(char *dbFilename) {
   FILE *file = fopen(dbFilename, "rb");
   if (file == NULL)
      return ID_BASE + 1;

   ACCOUNTDB temp;
   int maxId = ID_BASE;

   while (fread(&temp, sizeof(ACCOUNTDB), 1, file)) {
      int currentId = atoi(temp.account_id);
      if (currentId > maxId)
         maxId = currentId;
   }

   fclose(file);
   return maxId + 1;
}

/*
 * Funcion: loadAccounts
 * Objetivo: Leer el archivo binario en un arreglo dinamico, insertando cada record en orden alfabetico
 * Retorna: ACCOUNTLIST, que es una estructura contieniendo el arreglo ordenado y el contador de los record,
 *          si el archivo no puede ser abierto, retorna un arreglo vacio de ACCOUNTLIST
 */
ACCOUNTLIST loadAccounts(char *dbFilename) {
   ACCOUNTLIST list;
   list.records = NULL;
   list.count = 0;

   FILE *file = fopen(dbFilename, "rb");
   if (file == NULL)
      return list;

   ACCOUNTDB temp;

   while (fread(&temp, sizeof(ACCOUNTDB), 1, file)) {
      ACCOUNTDB *resized = (ACCOUNTDB *)realloc(list.records,
                                                (list.count + 1) * sizeof(ACCOUNTDB));
      if (resized == NULL) {
         fclose(file);
         return list;
      }

      list.records = resized;

      int insertPos = list.count;

      while (insertPos > 0) {
         ACCOUNTDB *candidate = &list.records[insertPos - 1];
         int nameCmp = strcmp(candidate->name, temp.name);
         int surnameCmp = strcmp(candidate->surname, temp.surname);

         if (nameCmp > 0 || (nameCmp == 0 && surnameCmp > 0))
            insertPos--;
         else
            break;
      }

      int shiftIndex = list.count;
      while (shiftIndex > insertPos) {
         list.records[shiftIndex] = list.records[shiftIndex - 1];
         shiftIndex--;
      }

      list.records[insertPos] = temp;
      list.count++;
   }

   fclose(file);
   return list;
}

/**
 * funcion: insertAccount
 * Objetivo: Genera un nuevo ID, presenta el formulario de ingreso,
 *           y si el usuario confirma, escribe el registro en el archivo binario.
 */
void insertAccount(char *dbFilename) {
   ACCOUNTDB newAccount;
   memset(&newAccount, 0, sizeof(ACCOUNTDB));

   int nextId = getLastId(dbFilename);
   sprintf(newAccount.account_id, "%08d", nextId);

   if (EditForm(&newAccount, 1)) {
      FILE *file = fopen(dbFilename, "ab");
      if (file != NULL) {
         fwrite(&newAccount, sizeof(ACCOUNTDB), 1, file);
         fclose(file);
      }
   }
}

/*
 * Funcion : deleteAccount
 * Objetivo: Carga los registros, presenta la tabla para que el usuario seleccione
 *           uno, pide confirmacion, y reescribe el archivo binario sin ese registro.
 *           Libera la memoria al terminar.
 */
void deleteAccount(char *dbFilename) {
   ACCOUNTLIST list = loadAccounts(dbFilename);

   if (list.count == 0) {
      system("cls");
      gotoxy(INIT_XPOS, INIT_YPOS);
      printf("No hay registros para eliminar.");
      getch();
      return;
   }

   int selectedIndex = MenuMovement(list, TABLE_XPOS, TABLE_YPOS, 0);

   if (selectedIndex == -1) {
      free(list.records);
      return;
   }

   system("cls");
   gotoxy(INIT_XPOS, INIT_YPOS);
   setColor(RED, BG_COLOR);
   printf("Eliminar: %s %s ? [S/N]: ",
          list.records[selectedIndex].name,
          list.records[selectedIndex].surname);
   setDefaultColor();

   int confirmKey = getch();

   if (confirmKey == 'S' || confirmKey == 's') {
      int shiftIndex = selectedIndex;
      while (shiftIndex < list.count - 1) {
         list.records[shiftIndex] = list.records[shiftIndex + 1];
         shiftIndex++;
      }
      list.count--;

      FILE *file = fopen(dbFilename, "wb");
      if (file != NULL) {
         fwrite(list.records, sizeof(ACCOUNTDB), list.count, file);
         fclose(file);
      }
   }

   free(list.records);
}

/*
 * Funcion : modifyAccount
 * Objetivo: Carga los registros, presenta la tabla para seleccionar uno,
 *           abre el formulario de edicion, y si el usuario guarda, busca
 *           el registro por ID en el archivo y lo sobreescribe en su posicion.
 *           Libera la memoria al terminar.
 */
void modifyAccount(char *dbFilename) {
   ACCOUNTLIST list = loadAccounts(dbFilename);

   if (list.count == 0) {
      system("cls");
      gotoxy(INIT_XPOS, INIT_YPOS);
      printf("No hay registros para modificar.");
      getch();
      return;
   }

   int selectedIndex = MenuMovement(list, TABLE_XPOS, TABLE_YPOS, 0);

   if (selectedIndex == -1) {
      free(list.records);
      return;
   }

   ACCOUNTDB editing = list.records[selectedIndex];

   if (EditForm(&editing, 0)) {
      FILE *file = fopen(dbFilename, "rb+");
      if (file != NULL) {
         ACCOUNTDB temp;
         int recordIndex = 0;

         while (fread(&temp, sizeof(ACCOUNTDB), 1, file)) {
            if (strcmp(temp.account_id, editing.account_id) == 0) {
               fseek(file, (long)(recordIndex) * sizeof(ACCOUNTDB), SEEK_SET);
               fwrite(&editing, sizeof(ACCOUNTDB), 1, file);
               break;
            }
            recordIndex++;
         }
         fclose(file);
      }
   }

   free(list.records);
}

/*
 * Funcion : scriptLogins
 * Objetivo: Carga los registros ordenados alfabeticamente, solicita al usuario
 *           un nombre de dominio, extrae el usuario del email de cada registro,
 *           y genera un archivo .sql con sentencias CREATE LOGIN en orden.
 * Formato : CREATE LOGIN [Dominio\Usuario] FROM WINDOWS;
 */
void scriptLogins(char *dbFilename, char *sqlFilename) {
   ACCOUNTLIST list = loadAccounts(dbFilename);

   if (list.count == 0) {
      system("cls");
      gotoxy(INIT_XPOS, INIT_YPOS);
      printf("No hay registros para exportar.");
      getch();
      return;
   }

   char domainName[MAX_NAME_LENGTH + 1];
   memset(domainName, 0, sizeof(domainName));

   system("cls");
   gotoxy(INIT_XPOS, INIT_YPOS);
   printf("Ingrese el nombre de dominio: ");
   gotoxy(INIT_XPOS + 30, INIT_YPOS);

   CaptureAlphanumeric(domainName, MAX_NAME_LENGTH,
                       INIT_XPOS + 30, INIT_YPOS,
                       validate_alphanumeric_char);

   FILE *sqlFile = fopen(sqlFilename, "w");
   if (sqlFile == NULL) {
      gotoxy(INIT_XPOS, INIT_YPOS + 2);
      printf("Error: No se pudo crear el archivo SQL.");
      getch();
      free(list.records);
      return;
   }

   int recordIndex = 0;
   while (recordIndex < list.count) {
      char username[MAX_EMAIL_LENGTH + 1];
      memset(username, 0, sizeof(username));

      char *atSymbol = strchr(list.records[recordIndex].email, '@');
      if (atSymbol != NULL) {
         int usernameLength = atSymbol - list.records[recordIndex].email;
         strncpy(username, list.records[recordIndex].email, usernameLength);
         username[usernameLength] = '\0';
      }

      fprintf(sqlFile, "CREATE LOGIN [%s\\%s] FROM WINDOWS;\n",
              domainName, username);
      recordIndex++;
   }

   fclose(sqlFile);
   free(list.records);

   system("cls");
   gotoxy(INIT_XPOS, INIT_YPOS);
   printf("Archivo generado: %s", sqlFilename);
   gotoxy(INIT_XPOS, INIT_YPOS + 1);
   printf("Presione cualquier tecla para continuar...");
   getch();
}

/**
 * funcion: EditForm
 * Objetivo: Presentar un formulario para ingresar o modificar un registro, se navega con las flechas de
 *           arriba o abajo, editar con ENTER, guardar con F2 o campo guardar. El campo ID es solo lectura. Valida formato de Email al confirmar
 *
 * Parametros: account: Puntero al registro a llenar o modificar, isNew: 1 si es un nuevo registroo, 0 si es una modificacion.
 * Retorna: 1 si el usuario se guardo, o 0 si se cancelo la modificacion.
 */
int EditForm(ACCOUNTDB *account, int isNew) {
   int activeField = FIELD_EMAIL;
   int pressedKey = 0;
   int saved = 0;
   char *errorMsg = NULL;

   do {
      show_form(account, activeField, errorMsg);
      errorMsg = NULL;

      pressedKey = getch();
      if (pressedKey == 0 || pressedKey == 224)
         pressedKey = getch();

      if (pressedKey == UP) {
         if (activeField > FIELD_EMAIL)
            activeField--;
      } else if (pressedKey == DOWN) {
         if (activeField < FIELD_SAVE)
            activeField++;
      }

      if (pressedKey == F2) {
         saved = 1;
         break;
      }

      if (pressedKey == ENTER) {
         switch (activeField) {
         case FIELD_ID:
            break;
         case FIELD_EMAIL:
            CaptureAlphanumeric(account->email, MAX_EMAIL_LENGTH,
                                INIT_XPOS + 20, FORM_YPOS + FIELD_EMAIL,
                                validate_email_char);
            if (!validate_email_format(account->email)) {
               memset(account->email, 0, MAX_EMAIL_LENGTH + 1);
               errorMsg = "Email invalido. Ejemplo: usuario@dominio.com";
            }
            break;
         case FIELD_NAME:
            CaptureAlphanumeric(account->name, MAX_NAME_LENGTH,
                                INIT_XPOS + 20, FORM_YPOS + FIELD_NAME,
                                validate_alpha_char);
            break;
         case FIELD_SURNAME:
            CaptureAlphanumeric(account->surname, MAX_SURNAME_LENGTH,
                                INIT_XPOS + 20, FORM_YPOS + FIELD_SURNAME,
                                validate_alpha_char);
            break;
         case FIELD_PWD:
            CaptureAlphanumeric(account->pwd, MAX_PWD_LENGTH,
                                INIT_XPOS + 20, FORM_YPOS + FIELD_PWD,
                                validate_alphanumeric_char);
            break;
         case FIELD_SAVE:
            saved = 1;
            break;
         }
      }

   } while (pressedKey != ESC);

   return saved;
}

/**
 * Funcion: show_form
 * Objetivo: Renderizar el formulario para modificar e introducir nuevas cuentas.
 * Paramemetros: Account: Puntero al registro actual, ActiveField: Indice del campo seleccionado, errorMsg: Mensaje de error a mostrar, NULL si no hay error.
 */
void show_form(ACCOUNTDB *account, int activeField, char *errorMsg) {
   char *labels[FIELD_COUNT] = {
       "ID:",
       "Email:",
       "Nombre(s):",
       "Apellido(s):",
       "Password:",
       "Guardar"};

   system("cls");
   gotoxy(INIT_XPOS, INIT_YPOS);
   setColor(TEXT_COLOR, BG_COLOR);
   printf("CRUD LOGINS | FORMULARIO");

   int fieldIndex = 0;
   while (fieldIndex < FIELD_COUNT) {
      gotoxy(INIT_XPOS, FORM_YPOS + fieldIndex);

      if (activeField == fieldIndex)
         setColor(SELECTED_TEXTCOLOR, SELECTED_BGCOLOR);
      else
         setColor(TEXT_COLOR, BG_COLOR);

      printf("%-18s", labels[fieldIndex]);
      gotoxy(INIT_XPOS + 20, FORM_YPOS + fieldIndex);

      switch (fieldIndex) {
      case FIELD_ID:
         printf("%s", account->account_id);
         break;
      case FIELD_EMAIL:
         printf("%s", account->email);
         break;
      case FIELD_NAME:
         printf("%s", account->name);
         break;
      case FIELD_SURNAME:
         printf("%s", account->surname);
         break;
      case FIELD_PWD:
         printf("%s", account->pwd);
         break;
      case FIELD_SAVE:
         if (activeField == FIELD_SAVE)
            printf("<< PRESIONE ENTER O F2 PARA GUARDAR >>");
         break;
      }

      setDefaultColor();
      fieldIndex++;
   }

   gotoxy(INIT_XPOS, FORM_YPOS + FIELD_COUNT + 1);
   setColor(TEXT_COLOR, BG_COLOR);
   printf("Navegar: %c%c | ENTER: Editar | F2: Guardar | ESC: Cancelar", ARROW_UP, ARROW_DOWN);

   gotoxy(INIT_XPOS, FORM_YPOS + FIELD_COUNT + 2);
   if (errorMsg != NULL) {
      setColor(RED, BG_COLOR);
      printf("%-50s", errorMsg);
   } else {
      printf("%-50s", " ");
   }

   setDefaultColor();
}

/*
 * Funcion: showOptions_Account
 * Objetivo: Renderiza la tabla de cuentas en pantalla con todas las columnas.
 * Parametros: list: Lista con los registros cargados en memoria, selectedIndex: indice de la fila actualmente resaltada, Xpos, Ypos: Posicion inicial de la tabla en pantalla.
 */
void showOptions_Account(ACCOUNTLIST list, int selectedIndex, int Xpos, int Ypos) {
   gotoxy(Xpos, Ypos);
   setColor(TEXT_COLOR, BG_COLOR);
   printf("%-10s %-62s %-27s %-22s", "ID", "Email", "Nombre", "Apellido(s)");

   int rowIndex = 0;
   while (rowIndex < list.count) {
      gotoxy(Xpos, Ypos + rowIndex + 1);

      if (selectedIndex == rowIndex)
         setColor(SELECTED_TEXTCOLOR, SELECTED_BGCOLOR);
      else
         setColor(TEXT_COLOR, BG_COLOR);

      printf("%-10s %-62s %-27s %-22s",
             list.records[rowIndex].account_id,
             list.records[rowIndex].email,
             list.records[rowIndex].name,
             list.records[rowIndex].surname);

      setDefaultColor();
      rowIndex++;
   }

   gotoxy(Xpos, Ypos + list.count + 1);
   setColor(TEXT_COLOR, BG_COLOR);
   printf("TOTAL: %d", list.count);
   setDefaultColor();
}

/*
 * Funcion : MenuMovement
 * Objetivo: Maneja la navegacion del usuario sobre la tabla de cuentas.
 *           UP/DOWN mueven la seleccion, ENTER confirma, ESC cancela.
 * Params  : list          — ACCOUNTLIST con los registros
 *           Xpos, Ypos    — posicion de la tabla
 *           selectedIndex — indice inicial de seleccion
 * Retorna : Indice seleccionado al presionar ENTER, o -1 si se presiono ESC.
 */
int MenuMovement(ACCOUNTLIST list, int Xpos, int Ypos, int selectedIndex) {
   int pressedKey = 0;

   _setcursortype(0);

   do {
      system("cls");
      showOptions_Account(list, selectedIndex, Xpos, Ypos);

      gotoxy(Xpos, Ypos + list.count + 2);
      setColor(TEXT_COLOR, BG_COLOR);
      printf("Navegar: %c%c | ENTER: Seleccionar | ESC: Cancelar", ARROW_UP, ARROW_DOWN);
      setDefaultColor();

      do {
         pressedKey = getch();
      } while (pressedKey != UP &&
               pressedKey != DOWN &&
               pressedKey != ENTER &&
               pressedKey != ESC);

      if (pressedKey == UP) {
         if (selectedIndex > 0)
            selectedIndex--;
         else
            selectedIndex = list.count - 1;
      } else if (pressedKey == DOWN) {
         if (selectedIndex < list.count - 1)
            selectedIndex++;
         else
            selectedIndex = 0;
      }

   } while (pressedKey != ENTER && pressedKey != ESC);

   _setcursortype(100);

   if (pressedKey == ESC)
      return -1;
   return selectedIndex;
}

/*
 * Funcion : show_header
 * Objetivo: Renderiza el menu principal horizontal en pantalla,
 *           resaltando la opcion actualmente seleccionada.
 * Params  : menu       — arreglo de strings con las opciones
 *           optCount   — numero de opciones
 *           Xpos, Ypos — posicion inicial en pantalla
 *           isSelected — indice de la opcion resaltada
 */
void show_header(char menu[][OPT_LENGTH], int optCount, int Xpos, int Ypos, int isSelected) {
   gotoxy(Xpos, Ypos);
   setColor(TEXT_COLOR, BG_COLOR);
   printf("CRUD LOGINS  ");

   int menuIndex = 0;
   while (menuIndex < optCount) {
      if (isSelected == menuIndex)
         setColor(SELECTED_TEXTCOLOR, SELECTED_BGCOLOR);
      else
         setColor(TEXT_COLOR, BG_COLOR);

      printf("%s  ", menu[menuIndex]);
      setDefaultColor();
      menuIndex++;
   }
}

/*
 * Funcion : CaptureAlphanumeric
 * Objetivo: Captura entrada de texto del usuario con soporte de navegacion
 *           tipo editor de texto: mover cursor, insertar en medio, borrar.
 *           Filtra caracteres usando la funcion validadora recibida.
 * Params  : result_str: buffer donde se almacena el resultado
 *           max_length: maximo de caracteres permitidos
 *           Xpos, Ypos: posicion en pantalla
 *           validator: funcion que determina si un caracter es valido
 */
void CaptureAlphanumeric(char *result_str, int max_length, int Xpos, int Ypos,
                         int (*validator)(char)) {
   int cursorIndex = 0;
   int pressedKey = 0;

   _setcursortype(100);

   do {
      show_field(result_str, cursorIndex, max_length, Xpos, Ypos);

      do {
         pressedKey = getch();
      } while (!validator(pressedKey) &&
               pressedKey != ENTER &&
               pressedKey != ESC &&
               pressedKey != LEFT &&
               pressedKey != BACKSPACE &&
               pressedKey != RIGHT);

      if (pressedKey == RIGHT) {
         if (cursorIndex < (int)strlen(result_str))
            cursorIndex++;
      } else if (pressedKey == LEFT) {
         if (cursorIndex > 0)
            cursorIndex--;
      } else if (pressedKey != ENTER && pressedKey != ESC) {
         if (pressedKey == BACKSPACE) {
            if (cursorIndex > 0) {
               cursorIndex--;
               strcpy(result_str + cursorIndex, result_str + cursorIndex + 1);
            }
         } else {
            int currentLength = strlen(result_str);
            if (currentLength < max_length) {
               int shiftIndex = currentLength;
               while (shiftIndex > cursorIndex) {
                  result_str[shiftIndex] = result_str[shiftIndex - 1];
                  shiftIndex--;
               }
               result_str[cursorIndex] = pressedKey;
               cursorIndex++;
            }
         }
      }

   } while (pressedKey != ENTER && pressedKey != ESC);

   if (pressedKey != ESC)
      result_str[cursorIndex] = '\0';
}

/*
 * funcion: show_field
 * Objetivo: Renderizar un campo de texto en pantalla, mostrando su conetenido y posicionando el cursor en la posicion actual del usuario.
 * Paremtros: field_str: Contenido actual del campo, cursor_position: posicion actual del cursor dentro del campo, max_length: longitud maxima del campo, pos_x y pos_y: Posicion en pantalla.
 */
void show_field(char *field_str, int cursor_position, int max_length, int pos_x, int pos_y) {
   setColor(TEXT_COLOR, BG_COLOR);

   int index = 0;
   while (index < max_length) {
      gotoxy(pos_x + index, pos_y);
      printf("%c", *(field_str + index) ? *(field_str + index) : ' ');
      index++;
   }

   setDefaultColor();
   gotoxy(pos_x + cursor_position, pos_y);
}

/*
 * Funcion : setColor
 * Objetivo: Establecer el color de texto y fondo en la consola.
 */
void setColor(int TextColor, int BackgroundColor) {
   textcolor(TextColor);
   textbackground(BackgroundColor);
}

/*
 * Funcion : setDefaultColor
 * Objetivo: Restablecer los colores de la consola a los valores por defecto.
 */
void setDefaultColor() {
   setColor(TEXT_COLOR, BG_COLOR);
}

/**
 * Funcion: validate_email_format
 * Objetivo: Verifica que el correo electronico este completo, y bien estructurado
 * Retorna: 1, si es valido. 0, si no es valido.
 */
int validate_email_format(char *email) {
   int atCount = 0;
   int atPosition = -1;
   int length = strlen(email);

   if (length == 0)
      return 0;

   int charIndex = 0;
   while (charIndex < length) {
      if (email[charIndex] == '@') {
         atCount++;
         atPosition = charIndex;
      }
      charIndex++;
   }

   if (atCount != 1)
      return 0;
   if (atPosition == 0)
      return 0;
   if (atPosition == length - 1)
      return 0;

   int dotFound = 0;
   int scanIndex = atPosition + 1;
   while (scanIndex < length) {
      if (email[scanIndex] == '.') {
         dotFound = 1;
         break;
      }
      scanIndex++;
   }

   return dotFound;
}

/**
 * Funcion: validate_email_char
 * Objetivo: Determinar si el caracter esta permitido en la entrada del correo.
 * Retorna: 1 si esta todo correcto, 0 si algo falla.
 */
int validate_email_char(char inputChar) {
   return (isalnum(inputChar) ||
           inputChar == '-' ||
           inputChar == '_' ||
           inputChar == '.' ||
           inputChar == '@');
}

/**
 * funcion: validate_alpha_char
 * Objetivo: Validar que la entrada del usuario no tenga numeros, ni simbolos.
 */
int validate_alpha_char(char inputChar) {
   return (isalpha(inputChar) || inputChar == ' ');
}

/*
 * funcion: validate_alphanumeric_char
 * Obejtivo: Validar que la entrada del usuario sean letras, numeros y espacios o guion.
 */
int validate_alphanumeric_char(char inputChar) {
   return (isalpha(inputChar) || isdigit(inputChar) ||
           inputChar == ' ' || inputChar == '-');
}
