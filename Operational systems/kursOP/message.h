#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct _msg{    
    int id;
    pid_t pid;
    int action;
    char name[10];
    void *requester;
    char text[256];
} Message;

#endif