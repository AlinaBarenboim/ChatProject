#ifndef _CHAT_SERVER_H_
#define _CHAT_SERVER_H_
#include "intern.h"




ChatServer* CharServerCreate(int _portNum, const char* _IP,size_t _buffSize,  CallBackFunc _callBack);
void ChatServerDestroy(ChatServer* _server);
void ChatServerRun(ChatServer* _server);

/*void SentToSpecificClient(Server_t* _server, void* _data, size_t _dataSize);*/

#endif
