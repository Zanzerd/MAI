  #include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "zmq.h"
#include "message.h"



void menuUser()
{
    printf("1) Put money into debit account\n");
    printf("2) Get money from debit account\n");
    printf("3) Put money into credit account\n");
    printf("4) Get money from credit account\n");
    printf("5) Leave the bank\n");
    printf("6) Check balance\n");
    printf("7) Send money to another account \n");
}

void *SendRecv(void *arg)
{
    MsgData *md = (MsgData *) arg;
    zmq_msg_t message;
    zmq_msg_init_size(&message, sizeof(MsgData));
    memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
    zmq_msg_send(&message, md->requester, 0);
    zmq_msg_close(&message);

    zmq_msg_init(&message);
    zmq_msg_recv(&message, md->requester, 0);
    md = (MsgData *) zmq_msg_data(&message);
    printf("%s\n", md->message);
    zmq_msg_close(&message);
    pthread_exit(NULL);
    return 0;
}

int main(int argc, char **argv)
{
  void *context = zmq_ctx_new();

  ID client;
  uint32_t bank;

    printf("Enter client's login: ");
    scanf("%lf", &client);


  char adress[25];
  printf("Enter bank's adress: ");
  scanf("%d", &bank);

  sprintf(adress, "%s%d", "tcp://localhost:", bank);

  printf("tcp://localhost:%d \n", bank);

  void *sendSocket = zmq_socket(context, ZMQ_REQ);
  zmq_connect(sendSocket, adress);

  int act = 0, sum = 0;
  menuUser();
  do {
    scanf("%d", &act);

    MsgData md;
    md.action = act;
    md.client = client;

    switch (act) {
      case 1: {
        printf("Enter the sum: ");
        scanf("%d", &sum);

        md.sum = sum;
        pthread_t th;
        md.requester = sendSocket;
        pthread_create(&th, NULL, SendRecv, &md);
        pthread_detach(th);
        break;
      }

      case 2: {
        printf("Enter the sum: ");
        scanf("%d", &sum);

        md.sum = sum;
        pthread_t th;
        md.requester = sendSocket;
        pthread_create(&th, NULL, SendRecv, &md);
        pthread_detach(th);

        break;
      }

      case 3: {
        printf("Enter the sum: ");
        scanf("%d", &sum);

        md.sum = sum;
        pthread_t th;
        md.requester = sendSocket;
        pthread_create(&th, NULL, SendRecv, &md);
        pthread_detach(th);

        break;
      }

      case 4: {
        printf("Enter the sum: ");
        scanf("%d", &sum);

        md.sum = sum;
        pthread_t th;
        md.requester = sendSocket;
        pthread_create(&th, NULL, SendRecv, &md);
        pthread_detach(th);

        break;
      }

      case 7: {
        ID receiverClient;
        printf("Enter receiver id: ");
        scanf("%lf", &receiverClient);

        printf("Enter the sum: ");
        scanf("%d", &sum);

        md.sum = sum;
        md.receiverClient = receiverClient;

        pthread_t th;
        md.requester = sendSocket;
        pthread_create(&th, NULL, SendRecv, &md);
        pthread_detach(th);

        break;
      }

      case 6: {
        pthread_t th;
        md.requester = sendSocket;
        pthread_create(&th, NULL, SendRecv, &md);
        pthread_detach(th);

        break;
      }

      case 5:
      break;

      default: {
        printf("Incorrect command\n");
        break;
      }
    }
  } while (act != 5);


  zmq_close(sendSocket);
  zmq_ctx_destroy(context);

  return 0;
}
