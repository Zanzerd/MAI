#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <zmq.h>
#include <signal.h>

#include "message.h"

int pid;
char text[100];
void* req;

void usage(){
	printf("hello!,this is an instant messenger,\n");
	printf("you can exit it by sending \'exit\' \n");
}
void send_recv(Message *mes)
{
    zmq_msg_t request;

    zmq_msg_init_size(&request, sizeof(Message));
    memcpy(zmq_msg_data(&request), mes, sizeof(Message));
    zmq_msg_send(&request, mes->requester, 0);
    zmq_msg_close(&request);

	zmq_msg_init(&request);
    zmq_msg_recv(&request, mes->requester, 0);
    mes = (Message*)zmq_msg_data(&request);
    zmq_msg_close(&request);
	if(mes->action==1){
		strcpy(text,mes->text);
	}
}

void printmsg(){
	Message mes;
	mes.action=1;
	mes.requester=req;
	send_recv(&mes);
	if(strcmp(text,"exit"))
		printf("%s\n",text);
}

int main (int argc, char *argv[])
{
	if(argc!=3){
		printf("enter ./client id name\n");
		return 0;
	}

    Message mes;
	void* context = zmq_ctx_new();
	if(context == NULL)
		return 0;
	void* request = zmq_socket(context, ZMQ_REQ);
	if(request == NULL)
		return 0;
	usage();
    zmq_connect(request,"tcp://localhost:4029");//create socket here   
	mes.pid=getpid();
	mes.requester=request;
	req=request;
	mes.action=-1;
    mes.id=atoi(argv[1]);
    strcpy(mes.name,argv[2]);
   	send_recv(&mes);
	signal(SIGINT,printmsg);
    do{
        gets(mes.text);
        mes.id=atoi(argv[1]);
		mes.action=0;
        mes.requester=request;
        send_recv(&mes);
    }
    while(strcmp(mes.text,"exit"));
    mes.id=atoi(argv[1]);
	zmq_close(request);
	zmq_ctx_destroy(context);

	return 0;
}
