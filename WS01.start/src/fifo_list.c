#include <fifo.h>
#include <list.h>
#include <stdio.h>
#include <stdlib.h>

struct list_type *list;

void init_queue() {
  list = malloc(sizeof(struct list_type));
  init_list(list);
}

void queue(void *element) { insert_head(list, element); }

void *dequeue() { return extract_tail(list); }

int size() { return list_size(list); }
