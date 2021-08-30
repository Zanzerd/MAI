#include "queue.h"

void Create(queue* q) {
  q->first = 0;
  q->size = 0;
}

int Empty(queue* q) {
  if (q->size == 0)
    return 1;
  else
    return 0;
}

int Size(queue* q) {
  return q->size;
}

int Push(queue* q, int cont) {
  if(q->size == POOL_SIZE)
    return 0;
  q->data[(q->first + q->size++) % POOL_SIZE] = cont;
  return 1;
}

int Pop(queue* q) {
  if (!q->size)
    return -1;
  int elem = q->data[q->first];
  q->first++;
  q->first %= POOL_SIZE;
  q->size--;
  return elem;
}

void Destroy(queue* q) {
  q->size = 0;
}

void Print (queue* q) {
  int i;
  printf("Очередь: ");
  for(i = q->first; i < (q->first + q->size)%POOL_SIZE; i++)
    printf("%d ", q->data[i]);
  printf("\n");
}
