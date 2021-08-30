#ifndef _BANK_H_
#define _BANK_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#define POOL_SIZE 100
#define USE_CREDIT 2
#define SUCCESS 1
#define NOT_MEMORY -1
#define NOT_ENOUGH_MONEY -2
#define NOT_CLIENT -3
#define RECEIVER_NOT_CLIENT -4

typedef double ID;

typedef struct _client {
    ID cli;
    int sum;
    int credit;
}_client; // Client - указатель на struct _client

typedef struct _account {
  struct _account* prev;
  struct _account* next;
  _client content;
}_account; // _account - ячейка в списке, клиент и связи с соседями

typedef struct Iterator {
 _account* point;
}Iterator;

typedef struct List {
  _account* head;
  _account* top;
  _account data[POOL_SIZE +1];
  uint32_t size;
  uint32_t freespace;
}List; // ClientDB - указатель на List

typedef List* ClientDB;

// функции работы с базой
void ClientDBCreate(ClientDB list);
void ClientAdd(ClientDB list, ID ClientID, int credit);
void ClientDBPrint(ClientDB list);
_client* ClientFind(ClientDB list, ID ClientID);
bool udt_is_empty(ClientDB list);

// функции работы с клиентами
void ClientAccIncrease(_client* cl, int sum);
void ClientDecreaseDebet(_client* cl, int sum);
void ClientDecreaseCredit(_client* cl, int credit);
int ClientHasEnoughMoney(_client* cl, int sum);
int ClientHasCredit(_client* cl, int credit);
void MoneyPut(ID ClientID, int sum, ClientDB cDB, int credarg); // тут был ClientInt
int MoneyGet(ID ClientID, int sum, ClientDB cDB); // тут был ClientInt
void MoneyPutCredit(ID ClientID, int sum, ClientDB cDB, int credarg);
int MoneyGetCredit(ID ClientID, int credit, ClientDB cDB, int credarg);
int MoneySend(ID clientSender, ID clientReceiver, int sum, ClientDB cDB, int credarg);
int CheckAccount(ID client, ClientDB cDB, int credarg);
int CheckCredit(ID client, ClientDB cDB);

#endif
