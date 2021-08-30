#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>
#include <error.h>
#include <errno.h>
#include "queue.h"


#define VERTEX_NUM 100

queue Qu;
unsigned int n;
unsigned int color[VERTEX_NUM];
int value[VERTEX_NUM];
int cycles = 0;
int matrix[VERTEX_NUM][VERTEX_NUM];

pthread_mutex_t mtx;


void* bfs(void* bfs_info);

int main(int argc, char* argv[])
{
  int i, j, m, res, maxt, thread_source, box;
  extern int errno;

  FILE* fd = fopen(argv[1], "r");
  if (fd == NULL) {
  perror("Unable to open file");
}
  fscanf(fd, "%d", &n);
  printf("кол-во вершин %d: 0..%d \n", n, n-1);

  int sournum = atol(argv[2]);
  printf("Второй аргумент командной строки: %d \n", sournum);
  if (sournum < 0 || sournum >= n) {
    printf("Второй аргумент должен быть номером вершины-источника \n");
    exit(EXIT_FAILURE);
  }
  for(i = 0; i < n; i++) {
    for(j = 0; j < n; j++)
    matrix[i][j] = -1;
  }

  for(j = 0; j < n; j++)
  fscanf(fd, "%d \n", &value[j]);
  printf("Значения вершин: \n");
  for(j = 0; j < n; j++)
  printf("%d-ая вершина: %d \n", j, value[j]);

  for(i = 0; i < n; i++) {
    for(j = 0; j < n; j++)
    fscanf(fd, "%d\n", &matrix[i][j]);
  }

  printf("Матрица смежности графа: \n");
  for (i = 0; i < n; i++)  // цикл по строкам
  {
    for (j = 0; j < n; j++)  // цикл по столбцам
    printf("%5d ", matrix[i][j]); // 5 знакомест под элемент массива
    printf("\n");
  }

  fclose(fd);

  res = pthread_mutex_init(&mtx, NULL);
  if (res != 0) {
    perror("Не удалось инициализировать мьютекс \n");
    exit(EXIT_FAILURE);
  }

  pthread_t* threads = (pthread_t*)malloc(n*sizeof(pthread_t));
  for (i = 0; i < n; i++) {
    color[i] = 0; // colors: 0 - white, 1 - gray, 2 - black
  }

  int source = sournum;
  color[source] = 1;
  //printf("source: %d its color: %d \n", source, color[source]);

  Create(&Qu);
  Push(&Qu, source);
  maxt = 1;

  while (!Empty(&Qu)) { // условие поменять?
    //Print(&Qu);
    //printf("Size %d \n", Size(&Qu));
    //usleep(50);
    for(i = 0; i < n && !Empty(&Qu); i++) {
      box = Pop(&Qu);
      if(box == -1) {
        printf("Очередь сломалась \n");
        break;
      }
      //pthread_mutex_lock(&mtx);
      if(pthread_create(&(threads[i]), NULL, bfs, &box) != 0) {
        fprintf(stderr, "pthread_create failed with i = %d. errno = %d, %s\n",
        i, errno, strerror(errno));
        break;
      }
      printf("Поток %d создан \n", box);
      //pthread_mutex_unlock(&mtx);
      color[box] = 2;
      //usleep(20);
      maxt = i+1;
    }
    //    printf("maxt: %d \n", maxt);
    //  printf("cycles: %d \n", cycles);

    for(i = 0; i<maxt; i++) {
      //pthread_mutex_lock(&mtx);
      res = pthread_join((threads[i]), NULL);
      //pthread_mutex_unlock(&mtx);
      if(res != 0) {
        fprintf(stderr, "pthread_join failed with i = %d. errno = %d, %s\n",
        i, errno, strerror(errno));
        break;
      }
    }
  }
  res = 0;
  for(i = 0; i < n; i++) {
    if(color[i] == 2)
    res++;
  }
  if (res == n)
    printf("Полный обход графа завершён \n");
  else {
    printf("Что-то не позволило обойти все вершины в графе \n");
    exit(EXIT_FAILURE);
  }
  if (cycles > 0) {
    printf("Есть по меньшей мере %d циклов \n", cycles);
  }
  else
  printf("Циклов нет \n");
  free(threads);
  pthread_mutex_destroy(&mtx);
  return 0;
}

void* bfs(void* thread_source) {

  int i, j, res;
  int* temp = (int*)thread_source;
  int source = *temp;
  printf("Поток %d начал работу \n", source);

  for(i = 0; i < n; i++) {
    if (matrix[source][i] == 1 && color[i] == 0) {
      pthread_mutex_lock(&mtx);
      color[i] = 1;
      //printf("Покрасил %d в цвет %d \n", i, color[i]);
      if (!Push(&Qu, i)) {
        printf("Очередь переполнилась \n");
        break;
      }
      else
      //printf("Запихнул %d-ую вершину в очередь \n", i);
      color[source] = 2;
      pthread_mutex_unlock(&mtx);
    }
    else if (matrix[source][i] == 1 && color[i] == 1){
      pthread_mutex_lock(&mtx);
      cycles++;
      color[source] = 2;
      //printf("Счётчик циклов: %d \n", cycles);
      pthread_mutex_unlock(&mtx);
    }
  }
  //printf("%d теперь цвета %d \n", source, color[source]);
  //for(i = 0; i< n; i++)
  //printf("№%d цвет %d \t", i, color[i]);
  //printf("\n");
  printf("Поток %d завершил работу \n", source);
  pthread_exit(NULL);
}
