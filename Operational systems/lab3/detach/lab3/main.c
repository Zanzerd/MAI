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

typedef struct Info{
  int source_ver;
  int*** matrix_ver;
  // struct Node* parent (?)
}Info;

queue Qu;
unsigned int n;
// unsigned int cur_source = 0;
unsigned int color[VERTEX_NUM];
int value[VERTEX_NUM];
int cycles;

pthread_mutex_t mtx;


void* bfs(void* bfs_info); // передавать в функцию на самом деле надо int**

int main(int argc, char* argv[]) // сделать source от argv[2]
{
  int i, j, m, res, maxt;
  extern int errno;
  Info for_thread;
  int** matrix;
  void* thread_res;
  FILE* fd = fopen(argv[1], "r");
  if (fd == NULL)
  perror("Unable to open file");
  int sournum = argv[2][0] - 48;
  printf("RAZMER PTHREAD_T and PTHREAD_T* : %zu %zu", sizeof(pthread_t), sizeof(pthread_t*));
  if (sournum < 0 || sournum > 9)
  printf("Второй аргумент должен быть номером вершины-источника \n");
  fscanf(fd, "%d", &n);
  printf("кол-во вершин %d \n", n);
  for(j = 0; j < n; j++)
  fscanf(fd, "%d \n", &value[j]);
  for(j = 0; j < n; j++)
  printf("%d-ая вершина: %d \n", j, value[j]);
  matrix = (int**)malloc(n * sizeof(int*));
  for(i = 0; i < n; i++) {
    matrix[i] = (int*)malloc(n * sizeof(int));
    for(j = 0; j < n; j++)
    fscanf(fd, "%d\n", &matrix[i][j]);
  }
  for (i = 0; i < n; i++)  // цикл по строкам
  {
    for (j = 0; j < n; j++)  // цикл по столбцам
    printf("%5d ", matrix[i][j]); // 5 знакомест под элемент массива
    printf("\n");
  }
  for_thread.matrix_ver = &matrix;
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

  printf("Вершина Цвет \n");
  for (i = 0; i < n; i++) {
    printf("%3d \t", i);
    printf("%3d \n", color[i]);

  }
  int cycles = 0;
  int source = sournum;
  int* store = (int*)malloc(n*sizeof(int));
  color[source] = 1;
  printf("source: %d %d \n", source, color[source]);
  Create(&Qu);
  Push(&Qu, source);

  while (!Empty(&Qu)) { // где-то в этом цикле всё и должно быть
    Print(&Qu);
    printf("Size %d \n", Size(&Qu));
    // осторожнее с мьютексами
      //usleep(50);
      for(i = 0; i < n && !Empty(&Qu); i++) {
      store[i] = Pop(&Qu);
      printf("массив: %d \n", store[i-1]);
      if(store[i] == -1) {
        printf("в сторе -1 \n");
        break;
      }
      for_thread.source_ver = store[i];
      pthread_mutex_lock(&mtx);
      printf("Mutex locked (main) \n");
      if(pthread_create(&(threads[i]), NULL, bfs, &for_thread) != 0) {
        fprintf(stderr, "pthread_create failed with i = %d. errno = %d, %s\n",
        i, errno, strerror(errno));
        maxt = i; /* Don't call join with uninitialised data */
        break;
      }
      printf("Поток %d создан \n", store[i]);
      pthread_mutex_unlock(&mtx);
      printf("Mutex unlocked (main) \n");
      maxt = i;
    }
    printf("maxt: %d \n", maxt);
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
      printf("Sleeping... \n");
      //sleep(1);
      //   sleep(1);

    for(i = 0; i<n; i++) {
    res = pthread_join((threads[i]), (thread_res));
        printf("Да что не так то сука!! \n");
      if(res != 0) {
        fprintf(stderr, "pthread_join failed with i = %d. errno = %d, %s\n",
        i, errno, strerror(errno));
        break;
      }
      if(thread_res != NULL) {
        printf("Что-то пошло не так, поток %d вернул не NULL \n ", store[i]);
      }
    }
    // где-то здесь сег фолт
  }
  //for (i = 0; i< maxt; i++) {
  //res = pthread_join((threads[i]), (thread_res));
  //if (res != 0 ) {
  // perror("Основная программа не смогла собрать потоки \n");
  //exit(EXIT_FAILURE);
  //}

  //printf("Поток вершины %d присоединён и вернул %d \n", i, thread_res);
  //}

  if (cycles > 0) {
    printf("Есть %d циклов", cycles);
  }

  pthread_mutex_destroy(&mtx);
  return 0;
}

void* bfs(void* bfs_info) {
  Info* local_info = (Info*)bfs_info;
  int i, res;
  //pthread_mutex_lock(&mtx);
  int source = local_info->source_ver;
  //pthread_mutex_unlock(&mtx);
  printf("Поток %d начал работу \n", source);
  for(i = 0; i < n; i++) {
    if (*(local_info->matrix_ver[source][i]) == 1 && color[i] == 0) {
      pthread_mutex_lock(&mtx);
      printf("Mutex locked \n");
      color[i] = 1;
      printf("Покрасил %d в цвет %d \n", i, color[i]);
      if (!Push(&Qu, i)) {
        printf("Очередь переполнилась \n");
        break;
      }
      else
      printf("Запихнул %d-ую вершину в очередь \n", i);
      color[source] = 2;
      pthread_mutex_unlock(&mtx);
      printf("Mutex unlocked \n");
    }
    else if (*(local_info->matrix_ver[source][i]) == 1 && color[i] == 1){
      pthread_mutex_lock(&mtx);
      printf("Mutex locked \n");
      cycles++;
      color[source] = 2;
      printf("Счётчик циклов: %d \n", cycles);
      pthread_mutex_unlock(&mtx);
      printf("Mutex unlocked \n");
    }
  }
  printf("%d теперь цвета %d \n", source, color[source]);
  for(i = 0; i< n; i++)
  printf("№%d цвет %d \t", i, color[i]);
  printf("\n");
  //printf("кол-во вершин: %d \n", n);
  printf("Поток %d завершил работу \n", source);
  pthread_exit(NULL);
}
