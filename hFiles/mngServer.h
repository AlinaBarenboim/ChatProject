#ifndef __MNG_SERVER_H__
#define __MNG_SERVER_H__
#include "intern.h"
typedef struct Server_t Server_t;
typedef struct ClientInfo ClientInfo; /*USER INFO*/
typedef void*(*CallBackFunc)(void* data,void* _context);

Server_t* ServerCreate(int _portNum, const char* _IP,size_t _buffSize,  CallBackFunc _callBack); /*config file*/
void ServerDestroy(Server_t* _server);
void ServerRun(Server_t* _server);

/*void SentToSpecificClient(Server_t* _server, void* _data, size_t _dataSize);*/

#endif
