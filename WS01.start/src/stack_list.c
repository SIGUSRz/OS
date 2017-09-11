#include <stack.h>
#include <list.h>
#include <stdlib.h>

struct list_type *list;

/* Initialization of the stack */
int init_stack() {
    list = malloc(sizeof(struct list_type));
    init_list(list);
    return size();
}

/* Insert an element */
int push(void *element) {
    insert_head(list, element);
    return size();
}

/* Extract an element */
void *pop() {
  return extract_head(list);
}

/* Compute the size of the stack  */
int size() { return list_size(list); }

int capacity() {return 0;}
