#include "people.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


People* find(People **head,int id){
    People *prev,*tmp=*head;
    if(*head==NULL){
        return NULL;
    }
    while(tmp!=NULL){
        prev=tmp;
        if(tmp->id==id){
            return tmp;
        }
        if(tmp!=NULL)
        tmp=tmp->next;
    }
    return NULL;
}
int insert_client(People **head,int id,pid_t pid,char *name){
    People *tmp=*head;
    People *prev;
    if(*head==NULL){
        (*head)=(People*)malloc(sizeof(People));
        (*head)->id=id;
        (*head)->pid=pid;
        strcpy((*head)->name,name);
        (*head)->next=NULL;
        return 0;
    }
    while(tmp!=NULL){
        if(tmp->id==id){
            return 1;
        }
        prev=tmp;
        tmp=tmp->next;
    }
    if(tmp==NULL){
        prev->next=(People*)malloc(sizeof(People));
        prev->next->id=id;
        prev->next->pid=pid;
        strcpy(prev->next->name,name);
        prev->next->next=NULL;
    }

    return 0;
}


void delete_client(People **head,int id){
	People *prev=*head,*tmp=*head,*tmp2=*head;
	while(tmp!=NULL){
		if(tmp->id==id)
			break;
		prev=tmp;
		tmp=tmp->next;
	}
	if(prev==tmp){
        (*head)=(*head)->next;
        free(tmp);
        if(*head==NULL)
            return;
	}
	else{
		prev->next=tmp->next;
		free(tmp);
	}
    strcpy((*head)->lastmsg,tmp2->lastmsg);
}
