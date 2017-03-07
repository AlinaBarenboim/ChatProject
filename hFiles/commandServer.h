#ifndef __COMMAND_SERVER_H__
#define __COMMAND_SERVER_H__



Server_t* ServerCreate(int _portNum, const char* _IP,size_t _buffSize,  CallBackFunc _callBack);
void ServerDestroy(Server_t* _server);
void ServerRun(Server_t* _server);

/*void SentToSpecificClient(Server_t* _server, void* _data, size_t _dataSize);*/

#endif
