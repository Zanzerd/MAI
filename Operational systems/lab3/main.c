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
  if (fd == NULL)
  perror("Unable to open file");

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
  for(j = 0; j < n; j++)
  printf("%d-ая вершина: %d \n", j, value[j]);

  //matrix = (int**)malloc(n * sizeof(int*));
  for(i = 0; i < n; i++) {
    //matrix[i] = (int*)malloc(n * sizeof(int));
    for(j = 0; j < n; j++)
    fscanf(fd, "%d\n", &matrix[i][j]);
  }

  for (i = 0; i < n; i++)  // цикл по строкам
  {
    for (j = 0; j < n; j++);  // цикл по столбцам
    //printf("%5d ", matrix[i][j]); // 5 знакомест под элемент массива
    //printf("\n");
  }

  //for_thread.matrix_ver = &(matrix);
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

  //printf("Вершина Цвет \n");
  for (i = 0; i < n; i++) {
    //printf("%3d \t", i);
    //printf("%3d \n", color[i]);
  }

  int source = sournum;
  //int* store = (int*)malloc(n*sizeof(int)); // мб сделать queue store?
  color[source] = 1;
  printf("source: %d its color: %d \n", source, color[source]);

  Create(&Qu);
  Push(&Qu, source);
  maxt = 1;

  while (!Empty(&Qu)) { // условие поменять?
    Print(&Qu);
    printf("Size %d \n", Size(&Qu));
    // осторожнее с мьютексами
    //usleep(50);
    //for(i = 0; i < n && !Empty(&Qu); i++) {
      box = Pop(&Qu);
      //printf("массив: %d \n", store[i]);
      if(box == -1) {
        printf("В СТОРЕ -1 \n");
        break;
      }
      pthread_mutex_lock(&mtx);
      //printf("Mutex locked (main) \n");
      if(pthread_create(&(threads[i]), NULL, bfs, &box) != 0) {
        fprintf(stderr, "pthread_create failed with i = %d. errno = %d, %s\n",
        i, errno, strerror(errno));
        break;
      }
      //printf("Массив ID потоков: ");
      //for(int c = 0; c < n; c++)
      //printf("%d \t", threads[i]);
      //printf("\n");
      printf("Поток %d создан \n", box);
      pthread_mutex_unlock(&mtx);
      //color[box] = 2;
      //usleep(20);
      //printf("Mutex unlocked (main) \n");
      maxt = i+1;
    printf("maxt: %d \n", maxt);
    printf("cycles: %d \n", cycles);
    //if(pthread_join(threads[i], &(thread_res)) != 0) {
    //fprintf(stderr, "pthread_join failed with i = %d. errno = %d, %s\n",
    // i, errno, strerror(errno));
    //maxt = i;
    //break;
    //}
    //if(res != 0) {
    //perror("Не получилось создать поток \n");
    //exit(EXIT_FAILURE);
    //}
    //sleep(1);
    //   sleep(1);

  //  for(i = 0; i<maxt; i++) {
      //pthread_mutex_lock(&mtx);
      res = pthread_join((threads[i]), NULL);
      //pthread_mutex_unlock(&mtx);
      if(res != 0) {
        fprintf(stderr, "pthread_join failed with i = %d. errno = %d, %s\n",
        i, errno, strerror(errno));
        break;
      }
  //  }
    // где-то здесь сег фолт
  //for (i = 0; i< maxt; i++) {
  //res = pthread_join((threads[i]), (thread_res));
  //if (res != 0 ) {
  // perror("Основная программа не смогла собрать потоки \n");
  //exit(EXIT_FAILURE);
  //}

  //printf("Поток вершины %d присоединён и вернул %d \n", i, thread_res);
  //}
}
  if (cycles > 0) {
    printf("Граф содержит циклы \n");
  }
  else
    printf("Циклов нет \n");
    free(threads);
  pthread_mutex_destroy(&mtx);
  return 0;
}

void* bfs(void* thread_source) {
  //Info* bfs_info = (Info*)info;
  int i, j, res;
  int* temp = (int*)thread_source;
  int source = *temp;
  printf("Поток %d начал работу \n", source);
  //sleep(1);
  //for (i = 0; i < n; i++)  // цикл по строкам
  //{
  //for (j = 0; j < n; j++)  // цикл по столбцам
  //printf("%5d ", matrix[i][j]); // 5 знакомест под элемент массива
  //printf("\n");
  //}
  //pthread_mutex_lock(&mtx);
  //pthread_mutex_unlock(&mtx);
  for(i = 0; i < n; i++) {
    if (matrix[source][i] == 1 && color[i] == 0) {
      pthread_mutex_lock(&mtx);
      //printf("Mutex locked \n");
      color[i] = 1;
      printf("Покрасил %d в цвет %d \n", i, color[i]);
      if (!Push(&Qu, i)) {
        printf("Очередь переполнилась \n");
        break;
      }
      else {
      printf("Запихнул %d-ую вершину в очередь \n", i);
    }
      pthread_mutex_unlock(&mtx);
      //printf("Mutex unlocked \n");
    }
    else if (matrix[source][i] == 1 && color[i] == 1){
      pthread_mutex_lock(&mtx);
      //printf("Mutex locked \n");
      cycles++;
      printf("Счётчик циклов: %d \n", cycles);
      pthread_mutex_unlock(&mtx);
      //printf("Mutex unlocked \n");
    }
  }
  color[source] = 2;
  printf("%d теперь цвета %d \n", source, color[source]);
  for(i = 0; i< n; i++)
  printf("№%d цвет %d \t", i, color[i]);
  printf("\n");
  //printf("кол-во вершин: %d \n", n);
  printf("Поток %d завершил работу \n", source);
  //sleep(2);
  pthread_exit(NULL);
}
