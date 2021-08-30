#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stropts.h>
#include <signal.h>

#include "zmq.h"
#include "bank.h"
#include "message.h"

volatile sig_atomic_t flag = 0;

void block_func(int sig)
{
  if (!flag) {
    flag = 1;
  } else {
    exit(0);
  }
}

void unblock_func(int sig)
{
  flag = 0;
}

void main(int argc, char** argv)
{
  List Database;
  int code, creditcode;
  int credarg;
  ClientDBCreate(&Database);
  ClientDB clientBase = &Database;
  void *context = zmq_ctx_new();
  void *responsSocket = zmq_socket(context, ZMQ_REP);
  printf("Hi! \n");
  if(argc != 2) {
    printf("Usage: program [credit sum available for clients] \n");
    return;
  } else {
    credarg = strtol(argv[1], NULL, 10);
    printf("Credit available: %d \n", credarg);
    if (credarg < 0) {
      printf("You must enter a positive number \n");
      return;
    }
  }
  char adress[25];
  printf("Enter bank's adress: ");
  uint32_t bank;
  scanf("%d", &bank);
  sprintf(adress, "%s%d", "tcp://*:", bank);

  zmq_bind(responsSocket, adress);

  while (1) {
    signal(SIGINT, block_func);
    signal(SIGTSTP, unblock_func);
    if (!flag) {

      zmq_msg_t message;

      zmq_msg_init(&message);
      zmq_msg_recv(&message, responsSocket, 0);
      MsgData *md = (MsgData *) zmq_msg_data(&message);
      zmq_msg_close(&message);

      char info[STR_SIZE];

      switch (md->action) {
        case 1: {
          printf("Put %d $ into the debit account id: %f\n", md->sum, md->client);
          MoneyPut(md->client, md->sum, clientBase, credarg);
          ClientDBPrint(clientBase);
          strcpy(info, "Operation was completed successfully\0");

          memcpy(md->message, info, strlen(info) + 1);
          zmq_msg_init_size(&message, sizeof(MsgData));
          memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
          zmq_msg_send(&message, responsSocket, 0);
          zmq_msg_close(&message);

          break;
        }

        case 2: {
          printf("Get money from the debit account id: %f\n", md->client);

          code = MoneyGet(md->client, md->sum, clientBase);
          if (code == SUCCESS) {
            printf("Success\n");
            ClientDBPrint(clientBase);
            strcpy(info, "Operation was completed successfully\0");
          } else if (code == USE_CREDIT) {
            printf("Success\n");
            ClientDBPrint(clientBase);
            strcpy(info, "Operation was completed with use of your credit account (not enough money on debit)\0");
          } else if (code == NOT_ENOUGH_MONEY) {
            printf("Not enough money\n");
            strcpy(info, "You don't have enough money\0");
          } else {
            printf("Not a bank client\n");
            strcpy(info, "You're not a bank client\0");
          }

          memcpy(md->message, info, strlen(info) + 1);
          zmq_msg_init_size(&message, sizeof(MsgData));
          memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
          zmq_msg_send(&message, responsSocket, 0);
          zmq_msg_close(&message);

          break;
        }
        case 3: {
          printf("Put %d $ into the credit account id: %f\n", md->sum, md->client);
          MoneyPutCredit(md->client, md->sum, clientBase, credarg);
          ClientDBPrint(clientBase);
          strcpy(info, "Operation was completed successfully\0");

          memcpy(md->message, info, strlen(info) + 1);
          zmq_msg_init_size(&message, sizeof(MsgData));
          memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
          zmq_msg_send(&message, responsSocket, 0);
          zmq_msg_close(&message);

          break;
        }

        case 4: {
          printf("Get money from the credit account id: %f\n", md->client);
          code = MoneyGetCredit(md->client, md->sum, clientBase, credarg);
          if (code == SUCCESS) {
            printf("Success\n");
            ClientDBPrint(clientBase);
            strcpy(info, "Operation was completed successfully\0");
          } else if (code == NOT_ENOUGH_MONEY) {
            printf("Not enough money\n");
            strcpy(info, "You don't have enough money\0");
          } else {
            printf("Not a bank client\n");
            strcpy(info, "You're not a bank client\0");
          }
          memcpy(md->message, info, strlen(info) + 1);
          zmq_msg_init_size(&message, sizeof(MsgData));
          memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
          zmq_msg_send(&message, responsSocket, 0);
          zmq_msg_close(&message);

          break;
        }

        case 7: {
          printf("Send money from account id: %f to account id: %f\n", md->client, md->receiverClient);
          code = MoneySend(md->client, md->receiverClient, md->sum, clientBase, credarg);
          if (code == SUCCESS) {
            printf("Success\n");
            ClientDBPrint(clientBase);
            strcpy(info, "Operation was completed successfully\0");
          } else if (code == USE_CREDIT)  {
            printf("Success\n");
            ClientDBPrint(clientBase);
            strcpy(info, "Operation was completed with use of your credit account (not enough money on debit)\0");
          } else if (code == NOT_ENOUGH_MONEY) {
            printf("Not enough money\n");
            strcpy(info, "You don't have enough money\0");
            ClientDBPrint(clientBase);
          } else if (code == RECEIVER_NOT_CLIENT) {
            printf("Receiver not bank client\n");
            strcpy(info, "Receiver is not a client of bank\0");
            ClientDBPrint(clientBase);
          } else {
          printf("Wtf happened \n");
        }

          memcpy(md->message, info, strlen(info) + 1);
          zmq_msg_init_size(&message, sizeof(MsgData));
          memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
          zmq_msg_send(&message, responsSocket, 0);
          zmq_msg_close(&message);

          break;
        }

        case 6: {
          printf("Check account id: %f\n", md->client);
          code = CheckAccount(md->client, clientBase, credarg);
          creditcode = CheckCredit(md->client, clientBase);
          if (code == NOT_CLIENT) {
            printf("Not bank client\n");
            strcpy(info, "You aren't client of bank\0");
          } else {
            printf("Client debit account is %d\n", code);
            printf("Client credit account is %d\n", creditcode);
            ClientDBPrint(clientBase);
            sprintf(info, "%s%d %s%d%c", "Your debit account is: ", code,
            "credit account is: ", creditcode, '\0');
          }

          memcpy(md->message, info, strlen(info) + 1);
          zmq_msg_init_size(&message, sizeof(MsgData));
          memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
          zmq_msg_send(&message, responsSocket, 0);
          zmq_msg_close(&message);

          break;
        }
      }
      zmq_msg_close(&message);
    }

  }
  zmq_close(responsSocket);
  zmq_ctx_destroy(context);

}
