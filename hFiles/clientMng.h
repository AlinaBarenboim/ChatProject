#ifndef __MNG_CLIENT_H__
#define __MNG_CLIENT_H__
#include <stddef.h>
#include "intern.h"
typedef struct Clients Clients;


void ClientsCreate(size_t _maxNumOfClients, int _portNum, const char* _IP);
void ClientsDestroy(Clients* _clients);
/*void ClientsRun(Clients* _clients);*/
void CreateNewConnection (int* _socket);
char* SendMessageToServer(int _socket, char* _payload);


#endif
