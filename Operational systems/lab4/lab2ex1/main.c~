#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "udt.h"


static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);

static int sem_id;

void copybuf(int* arr, int* buf);

union semun {
      int val;                  /* value for SETVAL */
      struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
      unsigned short *array;    /* array for GETALL, SETALL */
                                /* Linux specific part: */
      struct seminfo *__buf;    /* buffer for IPC_INFO */
};

int main() {
  int memFd = shm_open("example_memory", O_RDONLY, 0);
  if (memFd == -1)
  {
    perror("Can't open file");
    return 1;
  }

  int *buffer = mmap(NULL, sizeof(int)*3 , PROT_READ, MAP_SHARED, memFd, 0);
  if (buffer == NULL)
  {
    perror("Can't mmap");
    return -1;
  }
  //msync(NULL, sizeof(int)*3, MS_SYNC);
  sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
  if (sem_id == -1) {
    perror("Can't semget");
    return -1;
  }

  int i, act, pos, arg, k, a;
  int comm[3];
  List list;
  Iterator hole;

  udt_create(&list);
  if (udt_is_empty(&list))
  printf("Список пуст. \n");

  while(1) {
    if(semaphore_p()) {
    copybuf(comm, buffer);
    switch (comm[0]) {
      case 1:

      pos = comm[1];
      arg = comm[2];

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
      pos = comm[1];
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

      return 0;
      default:
      break;


    }
}
  }
  exit(EXIT_SUCCESS);
}

void copybuf(int* arr, int* buf) {
  for(int i = 0; i<3; i++)
  arr[i] = buf[i];
  semaphore_v();
  usleep(100);
}


static int set_semvalue(void) {
  union semun sem_union;
  sem_union.val = 1;
  if(semctl(sem_id, 0, SETVAL, sem_union) == -1)
  return 0;
  return 1;
}

static void del_semvalue(void) {
  union semun sem_union;
  if(semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
  fprintf(stderr, "Failed to delete semaphore\n");
}

static int semaphore_p(void) {
  struct sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = -1;
  sem_b.sem_flg = SEM_UNDO;
  if(semop(sem_id, &sem_b,1) == -1) {
    fprintf(stderr, "sem_p failed \n");
    return 0;
  }
  printf("main.c p-ed sem \n");
  return 1;
}

static int semaphore_v(void) {
  struct sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = 1;
  sem_b.sem_flg = SEM_UNDO;
  if(semop(sem_id, &sem_b,1) == -1) {
    fprintf(stderr, "sem_v failed \n");
    return 0;
  }
  printf("main.c v-ed sem \n");
  return 1;
}
