/*
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new() {
   queue_t *q = malloc(sizeof(queue_t));
   /* What if malloc returned NULL? */
   if (!q) {
      return NULL;
   }

   q->head = NULL;
   q->tail = NULL;
   q->size = 0;
   return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q) {
   if (!q) {
      return;
   }

   list_ele_t *curr = q->head;

   while (curr) {
      list_ele_t *next = curr->next;
      free(curr);
      curr = next;
   }

   free(q);
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
 */
bool q_insert_head(queue_t *q, int v) {
   list_ele_t *newh;
   /* What should you do if the q is NULL? */
   if (!q) {
      return false;
   }

   newh = malloc(sizeof(list_ele_t));
   /* What if malloc returned NULL? */
   if (!newh) {
      return false;
   }

   newh->value = v;
   newh->next = q->head;
   q->head = newh;

   if (!q->tail) {
      q->tail = newh;
   }
   q->size++;

   return true;
}

/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
 */
bool q_insert_tail(queue_t *q, int v) {
   /* You need to write the complete code for this function */
   /* Remember: It should operate in O(1) time */
   if (!q) {
      return false;
   }

   list_ele_t *newt = malloc(sizeof(list_ele_t));

   if (!newt) {
      return false;
   }

   newt->value = v;
   newt->next = NULL;

   if (!q->tail) {
      q->head = q->tail = newt;
   } else {
      q->tail->next = newt;
      q->tail = newt;
   }

   q->size++;

   return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If vp non-NULL and element removed, store removed value at *vp.
  Any unused storage should be freed
*/
bool q_remove_head(queue_t *q, int *vp) {
   if (!q || !q->head) {
      return false;
   }

   if (vp) {
      *vp = q->head->value;
   }

   list_ele_t *old = q->head;
   q->head = q->head->next;

   if (!q->head) {
      q->tail = NULL;
   }

   free(old);

   q->size--;

   return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q) {
   if (!q) {
      return 0;
   }

   return q->size;
}

/*
  Reverse elements in queue.

  Your implementation must not allocate or free any elements (e.g., by
  calling q_insert_head or q_remove_head).  Instead, it should modify
  the pointers in the existing data structure.
 */
void q_reverse(queue_t *q) {
   if (!q || !q->head) {
      return;
   }

   q->tail = q->head;

   list_ele_t *prev = NULL;
   list_ele_t *curr = q->head;

   while (curr) {
      list_ele_t *next = curr->next;

      curr->next = prev;
      prev = curr;
      curr = next;
   }

   q->head = prev;
}
