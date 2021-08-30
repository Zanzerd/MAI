#include "bank.h"


void ClientDBCreate(ClientDB list)
{
  int i;
  for (i = 0; i < POOL_SIZE; i++)
  list->data[i].next = &(list->data[i + 1]);
  list->data[POOL_SIZE - 1].next = NULL;
  list->head = &(list->data[POOL_SIZE]);
  list->head->prev = list->head;
  list->head->next = list->head;
  list->data[POOL_SIZE].content.cli = 1000;
  printf("%f \n", list->head->content.cli); // here
  list->top = &(list->data[0]);
  list->size = 0;
}

void ClientAdd(ClientDB list, ID ClientID, int credit)
{
  Iterator res;
  res.point = list->top;
  list->top->content.cli = ClientID;
  if (!res.point) {
    printf("Database is full \n");
    return;
  }
  list->top = list->top->next;
  //if (it->point == list->head) {
  if (udt_is_empty(list)) {
    res.point->next = list->head;
    res.point->prev = list->head;
    list->head->next = res.point;
    list->head->prev = res.point;
  }
  else {
    res.point->next = list->head;
    res.point->prev = list->head->prev;
    list->head->prev->next = res.point;
    list->head->prev = res.point;

  }
  //	}
  /*	else {
  res.point->next = it->point;
  res.point->prev = it->point->prev;
  res.point->prev->next = res.point;
  it->point->prev = res.point;
}*/

res.point->content.cli = ClientID;
res.point->content.sum = 0;
res.point->content.credit = credit;// с указателями не запутаться главное
list->size++;
}

void ClientDBPrint(ClientDB list)
{
  int i;
  Iterator res;
  if (udt_is_empty(list)) {
    printf("Database is empty \n");
    return;
  }
  printf("Database (List):  \n");
  printf("Client | Debit Balance | Credit Balance |\n");
  res.point = list->head->next; // ??
  for (i = 0; i < list->size; i++) {
    printf("%7.3f %7d %16d  \n", res.point->content.cli, res.point->content.sum, res.point->content.credit);
    res.point = res.point->next;
  }
  printf("\n");
}

_client* ClientFind(ClientDB list, ID ClientID)
{
  int i;
  Iterator res;
  if (udt_is_empty(list)) {
    printf("Database is empty \n");
    return NULL;
  }
  res.point = list->head->next;
  for(i=0; i < list->size; i++) {
    if(res.point->content.cli == ClientID)
    return (&(res.point->content)); // осторожно с адресами
    res.point = res.point->next;
  }
  printf("Cannot find the client \n");
  return NULL;
}

void ClientAccIncrease(_client* cl, int sum)
{
  cl->sum += sum;
}

void ClientCreditIncrease(_client* cl, int credit)
{
  cl->credit += credit;
}

void ClientDecreaseDebet(_client* cl, int sum)
{
  cl->sum -= sum;
}

void ClientDecreaseCredit(_client* cl, int credit)
{
  cl->credit -= credit;
}

int ClientHasEnoughMoney(_client* cl, int sum)
{
  return cl->sum >= sum;
}

int ClientHasCredit(_client* cl, int credit)
{
  return cl->credit >= credit;
}

void MoneyPut(ID ClientID, int sum, ClientDB cDB, int credarg)
{
  _client* cl = ClientFind(cDB, ClientID);

  if (cl) {
    ClientAccIncrease(cl, sum);
  } else {
    ClientAdd(cDB, ClientID, credarg);
    cl = ClientFind(cDB, ClientID);
    ClientAccIncrease(cl, sum);
  }
}

void MoneyPutCredit(ID ClientID, int sum, ClientDB cDB, int credarg)
{
  _client* cl = ClientFind(cDB, ClientID);

  if (cl) {
    ClientCreditIncrease(cl, sum);
  } else {
    ClientAdd(cDB, ClientID, credarg);
    cl = ClientFind(cDB, ClientID);
    ClientCreditIncrease(cl, sum);
  }
}

int MoneyGet(ID ClientID, int sum, ClientDB cDB)
{
  _client* cl = ClientFind(cDB, ClientID);
  if (!cl) {
    return NOT_CLIENT;
  }
  int credit = sum - cl->sum;
  if (ClientHasEnoughMoney(cl, sum)) {
    ClientDecreaseDebet(cl, sum);
    return SUCCESS;
  } else if (ClientHasCredit(cl, credit)) {
    ClientDecreaseDebet(cl, cl->sum);
    ClientDecreaseCredit(cl, credit);
    return USE_CREDIT;
  }
  else {
    return NOT_ENOUGH_MONEY;
  }
}

int MoneyGetCredit(ID ClientID, int credit, ClientDB cDB, int credarg)
{
  _client* cl = ClientFind(cDB, ClientID);
  if (!cl) {
    ClientAdd(cDB, ClientID, credarg);
    cl = ClientFind(cDB, ClientID);
  }
  if (ClientHasCredit(cl, credit)) {
    ClientDecreaseCredit(cl, credit);
    return SUCCESS;
  } else {
    return NOT_ENOUGH_MONEY;
  }
}

int MoneySend(ID clientSender, ID clientReceiver, int sum, ClientDB cDB, int credarg)
{
  _client* clSender = ClientFind(cDB, clientSender);
  if (!clSender) {
    ClientAdd(cDB, clientSender, credarg);
    clSender = ClientFind(cDB, clientSender);
    if(!clSender) {
      return NOT_CLIENT;
    }
  }
  _client* clReceiver = ClientFind(cDB, clientReceiver);
  if (!clReceiver) {
    return RECEIVER_NOT_CLIENT;
  }
  int credit = sum - clSender->sum;
  if (ClientHasEnoughMoney(clSender, sum)) {
    ClientDecreaseDebet(clSender, sum);
    ClientAccIncrease(clReceiver, sum);
    return SUCCESS;
  } else if (ClientHasCredit(clSender, credit)) {
    ClientDecreaseDebet(clSender, clSender->sum);
    ClientDecreaseCredit(clSender, credit);
    ClientAccIncrease(clReceiver, sum);
    return USE_CREDIT;
  }
  else {
    return NOT_ENOUGH_MONEY;
  }
}

int CheckAccount(ID client, ClientDB cDB, int credarg)
{
  _client* cl = ClientFind(cDB, client);
  if (!cl) {
    ClientAdd(cDB, client, credarg);
    cl = ClientFind(cDB, client);
    if (!cl) {
      printf("Something went terribly wrong \n");
      return NOT_CLIENT;
    }
  }
  return cl->sum;
}

int CheckCredit(ID client, ClientDB cDB)
{
  _client* cl = ClientFind(cDB, client);
  if (!cl) {
    return NOT_CLIENT;
  }
  return cl->credit;
}

bool udt_is_empty(ClientDB list) {
  return list->size == 0;
}
