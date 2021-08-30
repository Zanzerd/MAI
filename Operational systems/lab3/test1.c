#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int main() {
  queue qu;
  int i;
  int j = 0;
  Create(&qu);
  printf("%d \n", Size(&qu));
  for(i = 10; i<100; i+= 25) {
    Push(&qu, i);
    printf("%d \n", Size(&qu));
    j++;
  }
  while(j>0) {
    printf("%d \n", Pop(&qu));
    j--;
  }
  Destroy(&qu);
  if (Empty(&qu))
    printf("Очередь пуста \n");
  else
    printf("FFFFFFFFFFFFFF");

 
  
  return 0;
}
