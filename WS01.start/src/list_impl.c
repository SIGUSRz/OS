#include <list.h>
#include <stdio.h>
#include <stdlib.h>

void init_list(struct list_type *l) {
  l->head = NULL;
  l->tail = NULL;
  // Extra for Q7
  l->count = 0;
}

void insert_head(struct list_type *l, void *element) {
  cell *new_cell = malloc(sizeof(cell));
  new_cell->content = element;
  new_cell->previous = NULL;
  new_cell->next = l->head;
  if (l->head != NULL)
    l->head->previous = new_cell;
  l->head = new_cell;
  if (l->tail == NULL)
    l->tail = l->head;
  l->count++; // Extra for Q7
}

void *extract_head(struct list_type *l) {
  void *content = NULL;
  if (l->head == NULL) {
    printf("Cannot extract from empty linkedlist.\n");
    exit(0);
  }
  cell *next_node = NULL;
  next_node = l->head->next;
  content = l->head->content;
  free(l->head);
  l->head = next_node;
  l->count--; //Extra for Q7
  return content;
}

void *extract_tail(struct list_type *l) {
  void *content = NULL;
  if (l->tail == NULL) {
    printf("Cannot extract from empty linkedlist.\n");
    exit(0);
  }
  cell *prev_node = NULL;
  prev_node = l->tail->previous;
  content = l->tail->content;
  free(l->tail);
  l->tail = prev_node;
  l->count--; //Extra for Q7
  return content;
}

int list_size(struct list_type *l) {
  int counter = 0;
  cell *pointer = l->head;
  while (pointer != NULL) {
    counter++;
    pointer = pointer->next;
  }
  return counter;
}

// Extra for Q7
int list_size_alt(struct list_type *l) { return l->count; }
