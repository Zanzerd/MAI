#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "udt.h"
void main(int argc, char *argv[]) {
  int actr[3];
  int file_desc;

  int act, pos, arg, k, a;
  List list;
  Iterator hole;
  
  sscanf(argv[1], "%d", &file_desc);
  
  udt_create(&list);
  if (udt_is_empty(&list))
    printf("Список пуст. \n");
	

  while(1) {
    if(read(file_desc, actr, sizeof(actr)) < 0)
      perror("read");


    switch (actr[0]) {
    case 1:
      pos = actr[1];
      arg = actr[2];
      if ((pos < 1) || (udt_is_empty(&list) && pos != 1) || (pos > (list.size + 1))) {
	printf("Неверно введена позиция \n");
	break;
      }
      hole.point = list.head;
      for (k = 0; k < pos; k++)
	hole.point = hole.point->next;
      udt_insert(&list, &hole, arg);
      udt_print(&list);
      break;
    case 2:
      if (udt_is_empty(&list)) {
	printf("Список пуст \n");
	break;
      }
      pos = actr[1];
      if (pos < 1 || pos > list.size) {
	printf("Неверно введена позиция \n");
	break;
      }
      hole.point = list.head;
      for (k = 0; k < pos; k++)
	hole.point = hole.point->next;
      udt_erase(&list, &hole);
      udt_print(&list);
      break;
    case 3:
      udt_print(&list);
      break;
    case 4:
      a = udt_size(&list);
      printf("Размер списка: %d \n", a);
      break;
    case 5:
      hole.point = list.head;
      ins_sort(&list, &hole);
      udt_print(&list);
      break;
    case 6:
      wait(NULL);
      printf("Завершение программы \n");
      return;
    default:
      break;
    }
  }
}
