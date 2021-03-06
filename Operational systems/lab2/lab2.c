#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
  int pipe_fds[2];
  // int pipe_fds2[2] второй канал не пригодился
  int i;
  pid_t fork_res;
  char buffer[3];
  int act[3];

  if (pipe(pipe_fds) == 0) {
    fork_res = fork();
    if (fork_res == (pid_t)-1)
      perror("fork");
    if (fork_res == 0) {
      printf("Дочерний процесс %d \n", getpid());
      close(pipe_fds[0]); // дочерний не читает из канала
      while(1) {
	for(i = 0; i < 3; i++)
	  act[i] = 0;
	printf("МЕНЮ:\n");
	printf("1. Вставить элемент \n");
	printf("2. Удалить элемент \n");
	printf("3. Печать списка\n");
	printf("4. Размер списка\n");
	printf("5. Сортировать список\n");
	printf("6. Выход\n");
	scanf("%d", &act[0]);
	if (act[0] == 1) {
	  printf("Введите позицию элемента: \n");
	  scanf(" %d", &act[1]);
	  printf("Введите значение элемента: \n");
	  scanf(" %d", &act[2]);
	}
	if (act[0] == 2) {
	  printf("Введите позицию элемента: \n");
	  scanf(" %d", &act[1]);
	}
	if (act[0] < 1 || act[0] > 6)
	  printf("Такого пункта меню не существует");
	if(write(pipe_fds[1], act, sizeof(act)) < 0)
	  perror("write");
	if(act[0] == 6)
	  exit(EXIT_SUCCESS);
	usleep(50);
	
      }
    }
    else {
      printf("Процесс-родитель: %d \n", getpid());
      close(0); // родитель не читает стандартный ввод
      close(pipe_fds[1]); // родитель не пишет в канал
      sprintf(buffer, "%d", pipe_fds[0]);
      execl("/home/pankratz/Prog/OS/lab2/out", "out", buffer, NULL);
      perror("exec");
		
    }
    return 0;
  }
  else
    perror("pipe");
}

