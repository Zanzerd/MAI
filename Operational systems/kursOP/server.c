#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include <time.h>
#include <sys/ioctl.h>
#include <stropts.h>

#include "zmq.h"
#include "message.h"
#include "people.h"

volatile sig_atomic_t flag = 0;

void quit_server_func()
{
	printf("\nserver quit\n");
	flag = 3;
}

int main (int argc, char *argv[])
{
    People *list=NULL;
	zmq_msg_t request;
    zmq_msg_t reply;
	void* context = zmq_ctx_new();
	if(context == NULL)
		return 0;
    Message *mes;
	void* respond = zmq_socket(context, ZMQ_REP);
	if(respond == NULL)
		return 0;

	if(zmq_bind(respond, "tcp://*:4029")==-1)
			return 0;
	signal(SIGINT,quit_server_func);
	while(flag!=3) {
        printf("waiting for message\n");
		zmq_msg_init(&request);
		zmq_msg_recv(&request, respond, 0);
		mes=(Message*)zmq_msg_data(&request);
		zmq_msg_close(&request);
        if(mes->action==-1){
            if(insert_client(&list,mes->id,mes->pid,mes->name) == 1){
						printf("someone tried to hack %d's account'", mes->id);
						kill(mes->pid, SIGINT);
						//delete_client(&list,mes->id);
						//break;
				}
            zmq_msg_init_size(&reply,sizeof(Message));
            memcpy(zmq_msg_data(&reply),mes,sizeof(Message));
		    zmq_msg_send(&reply, respond, 0);
		    zmq_msg_close(&reply);
		    sprintf(list->lastmsg,"%s joined the conversation",mes->name);
        printf("number of people ++\n");
		    //People *tmp = list;
		    //while(tmp!=NULL){
		      //  if(tmp->pid!=mes->pid){
		        //    kill(tmp->pid,SIGINT);}
		        //tmp=tmp->next;
		        //if(tmp==NULL)break;
		    }

		else if(mes->action==1){
			zmq_msg_init_size(&reply,sizeof(Message));
			strcpy(mes->text,list->lastmsg);
        memcpy(zmq_msg_data(&reply),mes,sizeof(Message));
		    zmq_msg_send(&reply, respond, 0);
		    zmq_msg_close(&reply);
		}
        else{
		    if(!find(&list,mes->id)) {
				if(insert_client(&list,mes->id,mes->pid,mes->name) == 1) {
					printf("someone tried to hack %d's account \n", mes->id);
					kill(mes->pid, SIGINT);
				//break;
		}
	}
			if(!strcmp(mes->text,"exit"))
		    	sprintf(list->lastmsg,"%s %s from chat ",mes->name,mes->text);
			else
		    	sprintf(list->lastmsg,"%s | %s ",mes->name,mes->text);
            printf("id : %d ; name : %s ; message : %s \n",mes->id,mes->name,mes->text);
		    People *tmp = list;
		    while(tmp!=NULL){
		        if(tmp->pid!=mes->pid){
		            kill(tmp->pid,SIGINT);}
		        tmp=tmp->next;
		        if(tmp==NULL)break;
		    }
		  zmq_msg_init_size(&reply,sizeof(Message));
		  memcpy(zmq_msg_data(&reply),mes,sizeof(Message));
			zmq_msg_send(&reply, respond, 0);
			zmq_msg_close(&reply);
		  if(!strcmp(mes->text,"exit")){
		      delete_client(&list,mes->id);
		      if(list==NULL)
		          break;
		    }
        }
			}

	zmq_close(respond);
	zmq_ctx_destroy(context);

	return 0;
}
