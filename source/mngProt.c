#include <stdio.h>
#include <stdlib.h>
#include "mngProt.h"
#include "clientMng.h"
/*#define PORT 1291*/
/*#define IP "127.0.0.1"*/
#define MAX_NUM_OF_CLIENTS 100
/*define NDEBUG*/
/*define NTRACE*/
#define BUFF_SIZE 1024
#include <string.h>



char* StractToString (MngMsg* _mngmsg)
{
	char* dest;
	char devider[] = "|";
	char sockInChar[USER_NAME_SIZE];
	char OpCodeInChar [USER_NAME_SIZE];
	dest = malloc(BUFF_SIZE*sizeof(char));
	if (NULL == dest)
	{
		printf ("dest malloc fail \n");
		exit (EXIT_FAILURE);
	}
	sprintf (sockInChar, " %d" , _mngmsg->m_socketId );
	sprintf (OpCodeInChar,"%d", _mngmsg->m_opcode);
	strncpy (dest,  sockInChar, sizeof(sockInChar));
	strncat(dest, devider, sizeof(devider));
	strncat(dest, OpCodeInChar, sizeof(OpCodeInChar));
	strncat(dest, devider , sizeof(devider));
	strncat(dest, _mngmsg->m_userName, sizeof(_mngmsg->m_userName)*USER_NAME_SIZE);
	strncat(dest, devider , sizeof(devider));
	strncat(dest, _mngmsg->m_password , sizeof(_mngmsg->m_password)*PASSWORD_SIZE);
	strncat(dest, devider , sizeof(devider));
	strncat(dest, _mngmsg->m_data , sizeof(_mngmsg->m_data)*BUFF_SIZE);
	strncat(dest, devider , sizeof(devider));
	return dest;
}

	


MngMsg*  StringToStract (char* _payload) 
{
	MngMsg* mngmsg;
	char* sockInChar;
	char* token;
/*	printf ("payload in string to stract: %s \n", _payload);*/
	mngmsg = (MngMsg*)malloc(sizeof(MngMsg));
	if (NULL == mngmsg)
	{
		printf ("mngmsg null in string to mngmsg \n");
		exit (EXIT_FAILURE);
	}
	mngmsg->m_userName = malloc (USER_NAME_SIZE*sizeof(char));
	if (NULL == mngmsg->m_userName)
	{
		free (mngmsg);
		printf ("user name malloc error \n");
		exit (EXIT_FAILURE);
	}
	mngmsg->m_password = malloc (PASSWORD_SIZE*sizeof(char));
	if (NULL == mngmsg->m_password)
	{
		free (mngmsg->m_userName);
		free (mngmsg);
		printf ("user password malloc error \n");
		exit (EXIT_FAILURE);
	}
	mngmsg->m_data = calloc(BUFF_SIZE,sizeof(char));
	if (NULL == mngmsg->m_data)
	{
		free (mngmsg->m_userName);
		free (mngmsg->m_password);
		free (mngmsg);
		printf ("user data malloc error \n");
		exit (EXIT_FAILURE);
	}
	char* rest;
	sockInChar = strtok(_payload, "|");
	
/*	printf ("sock in char : %s \n", sockInChar);*/
	
	mngmsg->m_socketId = atoi(sockInChar);
/*	printf ("socket id after atoi : %d ", mngmsg->m_socketId);*/
	
	token = strtok (NULL, "|");
	mngmsg->m_opcode = atoi(token);
/*	printf ("opcode after atoi : %d \n", mngmsg->m_opcode);*/
	token = strtok (NULL, "|");
	mngmsg->m_userName = token;
/*	printf ("name : %s \n", mngmsg->m_userName);*/
	token = strtok (NULL, "|");
	mngmsg->m_password = token;
/*	printf ("password : %s \n", mngmsg->m_password);*/
	token = strtok (NULL, "|");
	mngmsg->m_data = token;
/*	printf ("data in string to stract: %s \n", mngmsg->m_data);*/
	return mngmsg;
}	

MngProtocol* CreateMngProtocol (const char* _configFile)
{
	
	ClientsCreate (MAX_NUM_OF_CLIENTS, PORT, IP);
}


MngMsg* SendMngMsg (MngMsg* _mngmsg, int* _sockId)
{
	char* payload;
	char* buffer;
	MngMsg* mngmsg;
	payload = malloc(BUFF_SIZE*sizeof(char));
	buffer = malloc(BUFF_SIZE*sizeof(char));
	if (_mngmsg->m_opcode == MNG_MSG_LOG_IN || _mngmsg->m_opcode == MNG_MSG_WELCOME || _mngmsg->m_opcode == MNG_MSG_SIGN_UP || _mngmsg->m_opcode == MNG_MSG_CHECK_IF_USER_EXIST)
	{
		CreateNewConnection (&_mngmsg->m_socketId);
		*_sockId = mngmsg->m_socketId;
	}
/*	printf ("SOCKET ID AFTER CREATING NEW CONNECTION: %d \n", _sockId);*/
	#ifndef NDEBUG
/*	printf ("mngmsg->m_socketId = %d \n", mngmsg->m_socketId);*/
/*	printf ("mngmsg->m_opcode = %d \n", mngmsg->m_opcode);*/
/*	printf ("mngmsg->m_userName = %s \n", mngmsg->m_userName);*/
/*	printf ("mngmsg->m_password = %s \n", mngmsg->m_password);*/
/*	printf ("mngmsg->m_data = %s \n", mngmsg->m_data);*/
	#endif
	payload = StractToString (_mngmsg);
	buffer = SendMessageToServer(_mngmsg->m_socketId,payload);
	mngmsg = StringToStract (buffer) ;
	#ifndef NDEBUG
/*	printf ("mngmsg->m_socketId = %d \n", mngmsg->m_socketId);*/
/*	printf ("mngmsg->m_opcode = %d \n", mngmsg->m_opcode);*/
/*	printf ("mngmsg->m_userName = %s \n", mngmsg->m_userName);*/
/*	printf ("mngmsg->m_password = %s \n", mngmsg->m_password);*/
/*	printf ("mngmsg->m_data = %s \n", mngmsg->m_data);*/
	#endif

	return mngmsg;

}
