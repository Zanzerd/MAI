#include <stdio.h>
#include "udt.h"

void udt_create(List* list) {
	int i;
	for (i = 0; i < POOL_SIZE; i++)
		list->data[i].next = &(list->data[i + 1]);
	list->data[POOL_SIZE - 1].next = 0;
	list->head = &(list->data[POOL_SIZE]);
	list->head->prev = list->head;
	list->head->next = list->head;
	list->head->content = -1000;
	list->top = &(list->data[0]);
	list->size = 0;
}
void udt_insert(List* list, Iterator* it, int cont) {
	Iterator res;
	res.point = list->top;
	if (!res.point) {
		printf("Список полон");
		return;
	}
	list->top = list->top->next;
	if (it->point == list->head) {
		if (udt_is_empty(list)) {
			res.point->next = list->head;
			res.point->prev = list->head;
			list->head->next = res.point;
			list->head->prev = res.point;
		}
		else {
			res.point->next = list->head;
			res.point->prev = list->head->prev;
			list->head->prev->next = res.point;
			list->head->prev = res.point;
	    
		}
	}
	else {
		res.point->next = it->point;
		res.point->prev = it->point->prev;
		res.point->prev->next = res.point;
		it->point->prev = res.point;
	}
	res.point->content = cont;
	list->size++;
}

void udt_erase(List* list, Iterator* it) {
	if (udt_is_empty(list)) {
		printf("Список пуст \n");
		return;
	}
	if (it->point == list->head) {
		printf("Голову списка удалить нельзя \n");
		return;
	}
	Iterator res;
	res.point = it->point->next;
	res.point->prev = it->point->prev;
	it->point->prev->next = res.point;
	list->size--;

	it->point->next = list->top;
	list->top = it->point;
	it->point = 0;
}

void udt_print(List* list) {
	int i;
	Iterator res;
	if (udt_is_empty(list)) {
		printf("Список пуст \n");
		return;
	}
	printf("Список: ");
	res.point = list->head->next; // ?? 
	for (i = 0; i < list->size; i++) {
		printf("%d ", res.point->content);
		res.point = res.point->next;
	}
	printf("\n");
}
bool udt_is_empty(List* list) {
	return list->size == 0;
}

int udt_size(List* list) {
	return(list->size);
}

int udt_fetch(List* list, Iterator* it) {
	return (it->point->content);
}

Item* udt_next(List* list, Iterator* it) {
	return (it->point->next);
}

Item* udt_prev(List* list, Iterator* it) {
	return (it->point->prev);
}


void procedure(List* list, Iterator* hole) {
	if (udt_is_empty(list))
		return;
	Iterator seek, kees, store;
	int box;
	int found = 0;
	seek.point = udt_next(list, hole);//seek.point = hole->point->next;
	while (seek.point != udt_prev(list,hole) && !found) {
		seek.point = udt_next(list, &seek);//seek.point = seek.point->next;
		store.point = udt_prev(list, &seek);
		if (udt_fetch(list, &seek) < udt_fetch(list, &store))
			found++;
	}
	if (found) {
		box = udt_fetch(list, &seek);
		kees.point = udt_prev(list, &seek);
		while ((store.point = udt_prev(list, &kees)) && (store.point != hole->point) && (box < udt_fetch(list, &store))) 
			kees.point = udt_prev(list, &kees);

		udt_erase(list, &seek);
		udt_insert(list, &kees, box);

	}
	else {
		return;
	}
}

void ins_sort(List* list, Iterator* hole) {
	int i;
	int a = udt_size(list);
	for (i = 1; i < a; i++)
		procedure(list, hole);
}
