#ifndef PEOPLE_H
#define PEOPLE_H
#include <unistd.h>

typedef struct _people{
    int id;
    pid_t pid;
    char name[10];
    char lastmsg[256];
    struct _people *next;
}People;

int insert_client(People**, int, pid_t,char*);
void delete_client(People**, int);
People* find(People** , int);

#endif
