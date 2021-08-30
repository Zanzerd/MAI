#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);

static int sem_id;

union semun {
      int val;                  /* value for SETVAL */
      struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
      unsigned short *array;    /* array for GETALL, SETALL */
                                /* Linux specific part: */
      struct seminfo *__buf;    /* buffer for IPC_INFO */
};

int main() {

  int i;

  pid_t fork_res = fork();
  if (fork_res == (pid_t)-1)
  perror("fork");
  if (fork_res == 0) {
    printf("Дочерний процесс %d \n", getpid());
    int memFd = shm_open("example_memory", O_CREAT | O_RDWR, S_IRWXU);
    if (memFd == -1)
    {
      perror("Can't open file");
      return 1;
    }

    int res = ftruncate(memFd, sizeof(int)*3);
    if (res == -1)
    {
      perror("Can't truncate file");
      return res;
    }
    int *buffer = mmap(NULL, sizeof(int)*3, PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0);
    if (buffer == NULL)
    {
      perror("Can't mmap");
      return -1;
    }

    sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
    if (sem_id == -1) {
      perror("Can't semget");
      return -1;
    }

    if (!set_semvalue()) {
      printf("AAA \n");
      fprintf(stderr, "Failed to initialize semaphore \n");
      exit(EXIT_FAILURE);
    }

    while(1) {

      if(semaphore_p()) {
      printf("МЕНЮ:\n");
      printf("1. Вставить элемент \n");
      printf("2. Удалить элемент \n");
      printf("3. Печать списка\n");
      printf("4. Размер списка\n");
      printf("5. Сортировать список\n");
      printf("6. Выход\n");
      scanf("%d", &buffer[0]);
      if (buffer[0] == 1) {
        printf("Введите позицию элемента: \n");
        scanf(" %d", &buffer[1]);
        printf("Введите значение элемента: \n");
        scanf(" %d", &buffer[2]);
      }
      if (buffer[0] == 2) {
        printf("Введите позицию элемента: \n");
        scanf(" %d", &buffer[1]);
      }
      if (buffer[0] < 1 || buffer[0] > 6)
      printf("Такого пункта меню не существует \n");

      semaphore_v();
      if(buffer[0] == 6) {
        del_semvalue();
        exit(EXIT_SUCCESS);
        // shm_unlink
      }

}
    }
  }
  else {
    printf("Процесс-родитель: %d \n", getpid());
    close(0); // родитель не читает стандартный ввод
    execl("/home/pankratz/Prog/OS/lab4/lab2ex/out", "out", NULL);
    perror("exec");

  }
  return 0;
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
  fprintf(stdout, "lab2.c p-ed sem \n");
  fflush(stdout);
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
  fprintf(stdout, "lab2.c v-ed sem \n");
  fflush(stdout);
  return 1;
}
