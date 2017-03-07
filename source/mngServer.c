#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include "mngServer.h"
#include <arpa/inet.h>
#include <unistd.h>
/*#include "/usr/src/linux-headers-4.4.0-38-generic/include/uapi/linux/tcp.h"*/
#include "../../advance_c/HashMap/HashMap.h"
/*#define IP "127.0.0.1"*/
/*#define PORT 3049*/
#define BUFF_SIZE 1026
#define ERROR 1
#define NUM_OF_CLIENTS 3
#define TIMEOUT 5
#define HASHMAP_CAP 1024
#define KEY_LEN 1000
#define NUM_OF_CONNECTIONS_WAITING 1
/*#include "ConfigServer.h"*/
#define MAX_CONNECTIONS 200
#define MAX_DIFF 10
#include <time.h> 
#include <signal.h>
/*define NDEBUG*/
/*define NTRACE*/


int isAlive = 1;

struct Server_t
{
	int m_serverSocket;
	HashMap* m_clientSockets;
	fd_set m_rfds;
	struct sockaddr_in m_serverAddr;
	int m_currSocket;
	int m_maxSocket;
	char* m_currSocketKey;
	int m_port;
	char* m_IP;
	int m_numOfConnections;
	CallBackFunc m_callback;
	void* m_buff;
	size_t m_buffSize;
	
};

struct ClientInfo
{
	int m_sockNum;
	time_t m_timeStamp;
};

static void KeyDestraction(void* _key)
{
	free(_key);
	return;
}

static void ValDestraction(void* _val)
{
	free(_val);
	return;
}

static void PrintAll (char* _key, ClientInfo* _value, Server_t* _server)
{
	printf("Printing key: %s , value: fd: %d, stamp: %lu\n", _key, _value->m_sockNum, _value->m_timeStamp);
	return;
}

static void CheckConnections(char* _key, ClientInfo* _value, Server_t* _server)
{
	time_t currentTime;
	double diff_t;
	time(&currentTime);
	ClientInfo* valToDestroy;
/*	printf("key: %s \n", _key);*/
/*	printf("value time stamp: %lu \n", _value->m_timeStamp);*/
/*	printf("value socket num: %d \n", _value->m_sockNum);*/
	diff_t = difftime(currentTime, _value->m_timeStamp);
	printf("Difference = %f\n", diff_t);
	if (diff_t > MAX_DIFF )
	{
/*		printf ("Socket number: %d destroyed after being inactive for 2 minutes\n", _value->m_sockNum);*/
		if (close (_value->m_sockNum) == -1)
		{
			perror ("close error: \n");
			return;
		}
	        FD_CLR(_value->m_sockNum, &(_server->m_rfds));
		
	
		if (HashMap_Remove(_server->m_clientSockets, _key,(void*) &valToDestroy) != MAP_SUCCESS)
		{
			printf ("Error while removing from hash \n");
			return;
		}
		
		ValDestraction(valToDestroy);
		_server->m_numOfConnections--;
	}
	return;
}


static void SaveNewClientSocket(Server_t* _server)
{
	ClientInfo* info;
	char* key;
	info = (ClientInfo*)malloc(sizeof(ClientInfo));
	if (NULL == info)
	{
		return;
	}
	key = (char*) malloc(KEY_LEN * sizeof(char)); 
	if (NULL == key)
	{
		free (info);
		return;
	}
	socklen_t len;
	struct sockaddr_storage addr;
	char ipstr[INET6_ADDRSTRLEN];
	int port;
	len = sizeof addr;
	
	time_t currentTime;
	time(&currentTime);
   	info->m_timeStamp=currentTime;
/*   	printf ("Time Stamp: %lu \n", info->m_timeStamp);*/
	
	getpeername(_server->m_currSocket, (struct sockaddr*)&addr, &len);

	
	if (addr.ss_family == AF_INET)
	{
		struct sockaddr_in *s = (struct sockaddr_in *)&addr;
		port = ntohs(s->sin_port);
		inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
	}
	else
	{ 
		struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
		port = ntohs(s->sin6_port);
		inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
	}

/*	printf("Peer IP address: %s\n", ipstr);*/
/*	printf("Peer port      : %d\n", port);*/
	
	sprintf(key, "%s%d%s%d",ipstr,port,_server->m_IP,_server->m_port);
/*	printf ("KEY: %s \n", key);*/
	
	info->m_sockNum = _server->m_currSocket;
	
	HashMap_Insert(_server->m_clientSockets,(void*)key, (void*)info);
/*	HashMap_ForEach(_server->m_clientSockets, (KeyValueActionFunction) PrintAll, _server);*/
	
	_server->m_numOfConnections ++;
	return;
}

static int HashFunc(char *str)
{
	int hash = 5381;
	int c;
	while (c = *(str++))
	hash = ((hash << 5) + hash) + c; 
	return hash;
}

static int FdSetFunc(char* _key, ClientInfo* _value, Server_t* _server)
{
	
	FD_SET(_value->m_sockNum, &_server->m_rfds);

	
	return 1;
}

static int FdIsSetFunc(char* _key, ClientInfo* _value, Server_t* _server)
{
	if(FD_ISSET(_value->m_sockNum, &_server->m_rfds) == 0)
	{
		return 1;
	}
	else
	{
		time_t newStamp;
		time(&newStamp);
		_server->m_currSocket = _value->m_sockNum;
		_server->m_currSocketKey = _key;
		_value->m_timeStamp = newStamp;
		return 0;
	}
	
/*	return (FD_ISSET(_value, &_server->m_rfds) == 0 ? 1 : 0);*/
}

static int KeyEq(char* _str1, char* _str2)
{
	return (strcmp(_str1, _str2) == 0);
}

void sig_handler(int signo, siginfo_t* sigDetails, char* sigContext)
{

          isAlive = 0;
          
            return;
}

Server_t* ServerCreate(int _portNum, const char* _IP,size_t _buffSize,  CallBackFunc _callBack)
{
	Server_t* server;
        
	server = (Server_t*) malloc(sizeof(Server_t));
	if (NULL == server)
	{
		return NULL;
	}
	server->m_buff = malloc(_buffSize);
	
	if (NULL == server->m_buff )
	{
		free (server);
		return NULL;
	}
	
	server->m_clientSockets = HashMap_Create(HASHMAP_CAP, (HashFunction) HashFunc, (EqualityFunction) KeyEq);
	
	server->m_port = _portNum;
	server->m_IP = /*_IP*/ INADDR_ANY;
	server->m_currSocket = 0;
	
	if((server->m_serverSocket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Error: couldn't open socket\n");
		free (server->m_buff);
		free (server);
		return NULL;
	}

	server->m_maxSocket = server->m_serverSocket;
	
	server->m_serverAddr.sin_family = AF_INET;
	server->m_serverAddr.sin_port = htons(_portNum);
	server->m_serverAddr.sin_addr.s_addr = /*inet_addr(_IP)htonl*/(INADDR_ANY);
	memset(server->m_serverAddr.sin_zero, 0, sizeof(server->m_serverAddr.sin_zero));
	
	if(bind(server->m_serverSocket, (struct sockaddr *) &server->m_serverAddr, sizeof(server->m_serverAddr)) == -1)
	{
		perror("Error: couldn't bind\n");
		free (server->m_buff);
		free (server);
		return NULL;
	}
	
	FD_ZERO(&server->m_rfds);
	FD_SET(server->m_serverSocket, &server->m_rfds);
	server->m_numOfConnections = 0;
	server->m_callback = _callBack;
	server->m_buffSize = _buffSize;
	
	return server;	
}

void ServerDestroy(Server_t* _server)
{
	if (close(_server->m_serverSocket) == -1)
	{
		perror ("error while destroy server \n");
		return;
	}
	printf ("Server Will Be Destroyed! \n");
	HashMap_Destroy(&_server->m_clientSockets, KeyDestraction, ValDestraction);
	free (_server->m_buff);
	free(_server);
}

void ServerRun(Server_t* _server)
{
	int result = 0;
	void* valToDestroy;
	int readBytesNum;
	char* buffer = NULL;
	buffer = malloc(_server->m_buffSize*sizeof(char));
	socklen_t addrSize = sizeof(struct sockaddr);
	
	listen(_server->m_serverSocket, NUM_OF_CONNECTIONS_WAITING);
	
	while(isAlive)/*FIXME*/
	{

		FD_ZERO(&_server->m_rfds);
		FD_SET(_server->m_serverSocket, &_server->m_rfds);
		
		HashMap_ForEach(_server->m_clientSockets, (KeyValueActionFunction) FdSetFunc, _server);
		
		result = select(_server->m_maxSocket + 1, &_server->m_rfds, NULL, NULL, NULL); /*TODO timeout*/
		
		if(result > 0)
		{

			if(FD_ISSET(_server->m_serverSocket, &_server->m_rfds) == 1)
			{

				_server->m_currSocket = accept(_server->m_serverSocket, (struct sockaddr*) &_server->m_serverAddr, &addrSize);
				SaveNewClientSocket(_server);
				if(_server->m_currSocket > _server->m_maxSocket)
				{
					_server->m_maxSocket = _server->m_currSocket;
				}
			}
			else
			{
				HashMap_ForEach(_server->m_clientSockets, (KeyValueActionFunction) FdIsSetFunc, _server);
				readBytesNum = read(_server->m_currSocket, (MngMsg*)buffer, _server->m_buffSize*sizeof(char));
				
				if(readBytesNum == 0)
				{
					if (close(_server->m_currSocket) == -1)
					{
						perror ("Close error : \n");
						return;
					}
					HashMap_Remove(_server->m_clientSockets, _server->m_currSocketKey, &valToDestroy);
					ValDestraction(valToDestroy);
					_server->m_numOfConnections--;

				}
				else if(readBytesNum > 0) /*server received message from client*/
				{
/*					write(_server->m_currSocket, buffer, strlen(buffer)+1);*/
					_server->m_callback(_server->m_currSocket, buffer);

				}
				else
				{
					perror ("read = -1: \n");
					/*FIXME */
					/*maybe silent close*/
					if (close(_server->m_currSocket) == -1)
					{
						perror ("Close Error : \n");
						return;
					}
					HashMap_Remove(_server->m_clientSockets, _server->m_currSocketKey, &valToDestroy);
					ValDestraction(valToDestroy);
/*					printf ("Socket number: %d destroyed , readBytes <0 \n", _server->m_currSocket);*/
					_server->m_numOfConnections--;
				}
			}
			
		}
		if (_server->m_numOfConnections > MAX_CONNECTIONS)
		{
/*			printf ("Cheks if some connections can be closed \n");*/
			HashMap_ForEach(_server->m_clientSockets, (KeyValueActionFunction) CheckConnections, _server);

		}
	}
}

int GetCurrentSocket (Server_t* _server)
{
	return _server->m_currSocket;
}

void PrintTestFunction (char* _str, void* _context)
{
	printf ("String Receiced : %s \n", _str);
	return;
}


















