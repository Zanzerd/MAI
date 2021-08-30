#ifndef UDT_H
#define UDT_H

#define POOL_SIZE 7

#include <stdbool.h>

typedef struct Item {
	struct Item* prev;
	struct Item* next;
	int content;
} Item;


typedef struct Iterator {
	Item* point;
} Iterator;

typedef struct List {
	Item* head;
	Item* top;
	Item data[POOL_SIZE + 1];
	int size;
} List;

void udt_create(List* list);
void udt_insert(List* list, Iterator* it, int cont);
void udt_erase(List* list, Iterator* it);
void udt_print(List* list);
bool udt_is_empty(List* list);
int udt_size(List* list);
int udt_fetch(List* list, Iterator* it);
Item* udt_next(List* list, Iterator* it);
Item* udt_prev(List* list, Iterator* it);

void procedure(List* list, Iterator* hole);
void ins_sort(List* list, Iterator* hole);

#endif