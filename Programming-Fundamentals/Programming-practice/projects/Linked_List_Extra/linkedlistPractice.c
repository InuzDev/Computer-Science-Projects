/* Quiz practice with linked list */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Word {
   char text[50];
};

struct Node {
   char *data;
   struct Node *prev;
   struct Node *next;
};

void CleanWord(char *input, char *output);
void InsertNode(struct Node **head, struct Node **tail, char *word);
int ExistsInList(struct Node *head, char *word);

int main() {
   struct Node *head = NULL;
   struct Node *tail = NULL;

   char buffer[128];
   char clean[128];

   FILE *file = fopen("test.dat", "rb");
   if (!file) {
      printf("Error opening file\n");
      return 1;
   }

   struct Word temp;

   while (fread(&temp, sizeof(struct Word), 1, file) == 1) {
      CleanWord(buffer, clean);
      if (strlen(clean) == 0) {
         continue;
      } else if (!ExistsInList(head, clean)) {
         InsertNode(&head, &tail, clean);
      }
   }

   fclose(file);

   struct Node *current = head;

   while (current) {
      struct Node *next = current->next;

      free(current->data);
      free(current);

      current = next;
   }

   return 0;
}

void CleanWord(char *input, char *output) {
   int j = 0;
   for (int index = 0; input[index]; index++) {
      if (isalpha(input[index])) {
         output[j++] = tolower(input[index]);
      }
   }
   output[j] = '\0';
}

// Creating a new node.
void InsertNode(struct Node **head, struct Node **tail, char *word) {
   struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));

   newNode->data = (char *)malloc(strlen(word) + 1);
   strcpy(newNode->data, word);

   newNode->next = NULL;
   newNode->prev = *tail;

   if (*tail) {
      (*tail)->next = newNode;
   } else {
      *head = newNode;
   }

   *tail = newNode;
}

int ExistsInList(struct Node *head, char *word) {
   struct Node *current = head;

   while (current) {
      if (strcmp(current->data, word) == 0) {
         return 1;
      }
      current = current->next;
   }
   return 0;
}
