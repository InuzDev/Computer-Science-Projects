// Example of linked list - geeksforgeeks
#include <stdio.h>
#include <stdlib.h>

struct Node {
   int data;
   struct Node *next;
};

int main(void) {
   // Create the nodes
   struct Node *head = NULL;
   struct Node *second = NULL;
   struct Node *third = NULL;

   // Allocate the nodes in memory
   head = (struct Node *)malloc(sizeof(struct Node));
   second = (struct Node *)malloc(sizeof(struct Node));
   third = (struct Node *)malloc(sizeof(struct Node));

   // Check the allocation
   if (!head || !second || !third) {
      printf("Memory allocation failed.\n");
      return 1;
   }

   // Assign data and link nodes.
   head->data = 10;
   head->next = second;

   second->data = 20;
   second->next = third;

   third->data = 342;
   third->next = head;

   // Print Linked list
   struct Node *temp = NULL;
   while (temp != NULL) {
      printf("%d -> ", temp->data);
      temp = temp->next;
   }
   printf("NULL\n");

   // Free allocated memory.
   temp = head;
   while (temp != NULL) {
      struct Node *nextNode = temp->next;
      free(temp);
      temp = nextNode;
   }

   return 0;
}
