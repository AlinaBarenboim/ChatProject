#include <sys/types.h>        
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFF_SIZE 1024
/*#define IP "127.0.0.1"*/
#define ERROR 1
/*#define PORT 3045*/
#define NUM_OF_CLIENTS 3
/*#include "/usr/src/linux-headers-4.4.0-38-generic/include/uapi/linux/tcp.h"*/
#include "clientMng.h"
#define CLIENT_NOT_CONNECTED 0
/*#include "ConfigClient.h"*/
#define MIN_CLIENTS_NUM 3
#define TIMEOUT 1
/*define NDEBUG*/
/*define NTRACE*/


struct Clients
{
	struct sockaddr_in m_serverAddr;
	socklen_t m_addr_size;
	size_t m_maxNumumOfClients; /*FIXME maybe?*/
	fd_set m_rfds;
};

Clients* clientsStruct;

static char* ReceiveMessage ( int* _socket)
{
	int result;
	char* buffer;
	struct timeval timeout;
	timeout.tv_sec = TIMEOUT;
        timeout.tv_usec = 0;
	FD_ZERO(&clientsStruct->m_rfds);
	FD_SET(*_socket, &clientsStruct->m_rfds);
	buffer = malloc(BUFF_SIZE*sizeof(char));

		result = recv(*_socket, buffer,BUFF_SIZE, 0);
		if (result == 0)
		{
			close(*_socket);
			*_socket = 0;
			#ifndef NDEBUG
			printf ("Found dead socket and closed!!! \n");
			#endif
		}
		else if (result < 0)
		{
			perror("result of recv < 0:  \n");
		}
		else
		{
			#ifndef NDEBUG
/*			printf ("data from socket num %d received : %s \n", *_socket, buffer);*/
			#endif
		
		}
	
		return buffer;

}


char* SendMessageToServer(int _socket, char* _payload)
{
	char* buffer;
	int numOfWrittenBites;
	int readBytes;
	int result;
        int len;
        #ifndef NDEBUG
/*	printf("Sending Message \n");*/
/*	printf ("String from client number %d  \n", _socket);*/
	#endif
	buffer = malloc(BUFF_SIZE*sizeof(char));
	if (NULL == buffer)
	{
		printf("buffer null: \n");
		exit (EXIT_FAILURE);
	}
	len = strlen(_payload)+1;
	if (numOfWrittenBites = write (_socket, _payload, len)<= 0) /*TODO dynamic size*/
	{
		perror ("error write to socket : \n");
		exit (EXIT_FAILURE);
	}
	buffer = ReceiveMessage (&_socket);
/*	printf ("Buffer in send msg to server: %s \n", buffer);*/
	return buffer;
	
}

void CreateNewConnection (int* _socket)
{
	if((*_socket = socket(PF_INET, SOCK_STREAM  /*| SOCK_NONBLOCK*/, 0)) == -1)
	{
		perror("Error open socket");
		return;
	}
	if(connect(*_socket, (struct sockaddr *) &clientsStruct->m_serverAddr, clientsStruct->m_addr_size) == -1)
	{
		perror("Error in connection with the server");
		return;
	}
/*	printf ("new sock num: %d \n",*_socket);*/
	#ifndef NDEBUG
/*	printf("New Connection Created \n");*/
	#endif
	
}


void ClientsCreate (size_t _maxNumOfClients, int _portNum, const char* _IP)
{
	

	clientsStruct = (Clients*)malloc(sizeof(Clients));
	if (NULL == clientsStruct)
	{
		printf ("clients malloc error \n");
		exit (EXIT_FAILURE);
	}
	
	clientsStruct->m_serverAddr.sin_family = AF_INET;
	clientsStruct->m_serverAddr.sin_port = htons(_portNum);
	clientsStruct->m_serverAddr.sin_addr.s_addr = inet_addr(_IP/*"10.0.0.13"*/);
	
	memset(clientsStruct->m_serverAddr.sin_zero, '\0', sizeof(clientsStruct->m_serverAddr.sin_zero));

	clientsStruct->m_addr_size = sizeof(clientsStruct->m_serverAddr);	
	clientsStruct->m_maxNumumOfClients = 0;
	
	FD_ZERO(&clientsStruct->m_rfds);
	#ifndef NDEBUG
/*	printf("Clients Created \n");*/
	#endif

	return ;
}


