#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#define POOL_SIZE 100

typedef struct queue {
  int first;
  int size;
  int data[POOL_SIZE];
} queue;


void Create(queue* q);
int Empty(queue* q);
int Size(queue* q);
int Push(queue* q, int cont);
int Pop(queue* q);
void Destroy(queue* q);
void Print(queue* q);

#endif
