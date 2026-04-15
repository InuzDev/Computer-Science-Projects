// Double linked list example
#include <stdio.h>
#include <stdlib.h>

struct Node {
   int data;
   struct Node *next;
   struct Node *prev;
};

void AssignDataNodes(struct Node node, int userInput);

int main() {
   // Create the pointers
   struct Node *head = NULL;
   struct Node *second = NULL;
   struct Node *third = NULL;

   // Allocate in memory the three nodes
   head = (struct Node *)malloc(sizeof(struct Node));
   second = (struct Node *)malloc(sizeof(struct Node));
   third = (struct Node *)malloc(sizeof(struct Node));

   if (!head || !second || !third) {
      printf("Error in memory allocation\n");
      return 1;
   }

   int ValueInput = 0;
   printf("Get user input for nodes [Default 0]: ");
   scanf("%d", &ValueInput);

   // I will try to make the data assignation more dynamic.
   // AssignDataNodes(int *Node, int userInput);

   return 0;
}

void AssignDataNodes(struct Node node, int userInput) {

   return;
}
