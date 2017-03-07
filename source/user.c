#include <stdio.h>
#include <stdlib.h>
#include "intern.h"
#include "user.h"
#include <unistd.h>
#include "superUser.h"
#include "userInterface.h"
#include "mngProt.h"
#define SUPER_USER_NAME "Alina"
#define SUPER_USER_PASSWORD "alina123"
#define WRONG_KEY 0xdeadbeef
#define BUFF_SIZE 1024
/*define NDEBUG*/
/*define NTRACE*/
#include <string.h>

static MngMsg* AllocateMemoryForMngMsg()
{

	MngMsg* mngmsg = NULL;
	mngmsg = (MngMsg*)malloc(sizeof(MngMsg));
	if (NULL == mngmsg)
	{
		
		printf ("mngmsg malloc error \n");
		exit (EXIT_FAILURE);
	}
	mngmsg->m_opcode = MNG_MSG_CHECK_IF_USER_EXIST;
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
	return mngmsg;
}



static MngMsg* CreateWelcomeMsg(void)
{
	MngMsg* mngmsg = NULL;
	mngmsg = AllocateMemoryForMngMsg();
	mngmsg->m_userName = "*";
	mngmsg->m_data = "*";
	mngmsg->m_opcode = MNG_MSG_WELCOME;
	mngmsg->m_password = "*";
/*	mngmsg->m_socketId = (int)"*";*/
	return mngmsg;

}

static MngMsg* CreateShowAllMyGroupsMngMessage(char* _userName, int _sockId) /*FIXME*/
{
	MngMsg* mngmsg = NULL;
	mngmsg = AllocateMemoryForMngMsg();
	mngmsg->m_userName = _userName;
	mngmsg->m_data = "*";
	mngmsg->m_opcode = MNG_MSG_SHOW_ALL_MY_GROUPS;
	mngmsg->m_password = "*";
	mngmsg->m_socketId =_sockId;
	return mngmsg;



}






static MngMsg* CreateStartChatMngMessage(char* _userName, char* _groupName, int _sockId)
{
	MngMsg* mngmsg = NULL;
	mngmsg = AllocateMemoryForMngMsg();
	mngmsg->m_userName = _userName;
	mngmsg->m_data = _groupName;
	mngmsg->m_opcode = MNG_MSG_START_CHAT;
	mngmsg->m_password = "*";
	mngmsg->m_socketId = _sockId;
	
	return mngmsg;
}


static MngMsg*  CreateJoinGroupMngMessage(char* _userName,char* _groupName, int _sockId)
{
	MngMsg* mngmsg = NULL;
	mngmsg = AllocateMemoryForMngMsg();
	mngmsg->m_userName = _userName;
	mngmsg->m_data = _groupName;
	mngmsg->m_opcode = MNG_MSG_JOIN_GROUP;
	mngmsg->m_password = "*";
	mngmsg->m_socketId = _sockId;
	return mngmsg;
}




static MngMsg* CreateNewGroupMngMessage(char* _userName, char* _groupName, int _sockId)
{
	MngMsg* mngmsg = NULL;
	mngmsg = AllocateMemoryForMngMsg();
	mngmsg->m_userName = _userName;
	mngmsg->m_data = _groupName;
	mngmsg->m_opcode = MNG_MSG_CREATE_GROUP;
	mngmsg->m_password = "*";
	mngmsg->m_socketId = _sockId;
	return mngmsg;
}

static MngMsg* CreateLogInMngMessage(char* _userName,char*  _password)
{
	MngMsg* mngmsg = NULL;
	mngmsg = AllocateMemoryForMngMsg();
	mngmsg->m_userName = _userName;
	mngmsg->m_password = _password;
	mngmsg->m_opcode = MNG_MSG_LOG_IN;
/*	mngmsg->m_socketId = (int)"*";*/
	mngmsg->m_data = "*";
	return mngmsg;
}


static MngMsg* CreateCheckIfUserExistMngMessage (char* _userName)
{
	MngMsg* mngmsg = NULL;
	mngmsg = AllocateMemoryForMngMsg();
	mngmsg->m_userName = _userName;
	mngmsg->m_opcode = MNG_MSG_CHECK_IF_USER_EXIST;
/*	mngmsg->m_socketId = "*";*/
	mngmsg->m_data = "*";
	mngmsg->m_password = "*";
	return mngmsg;
}




static MngMsg* CreateSignUpMngMessage (char* _userName, char* _password)
{
	MngMsg* mngmsg = NULL;
	mngmsg = AllocateMemoryForMngMsg();
	mngmsg->m_userName = _userName;
	mngmsg->m_password = _password;
	mngmsg->m_opcode = MNG_MSG_SIGN_UP;
/*	mngmsg->m_socketId = "*";*/
	mngmsg->m_data = "*";
	return mngmsg;	
}


MngMsg* UserSignUp ( char* _userName, char* _password, int _sockId)
{
	MngMsg* mngmsg = NULL;
	mngmsg = CreateSignUpMngMessage(_userName, _password);
	mngmsg = SendMngMsg (mngmsg, &_sockId);
	return mngmsg;

}


int CheckIfUserExist (char* _userName, int _sockId)
{

	MngMsg* mngmsg = NULL;
	mngmsg = CreateCheckIfUserExistMngMessage(_userName);
	mngmsg = SendMngMsg (mngmsg, &_sockId);
	if (strcmp(mngmsg->m_data,"ok" )!=0)
	{
		free (mngmsg);
		return NOT_EXIST;
	}
	free (mngmsg);
	return EXIST;
}

Status UserLogIn ( char* _userName, char* _password, int* _sockId)
{
	MngMsg* mngmsg = NULL;
	int socket;
	
	mngmsg = CreateLogInMngMessage(_userName, _password);
	mngmsg = SendMngMsg(mngmsg, &socket);
	if (strcmp(mngmsg->m_data,"ok" )!=0)
	{
		free (mngmsg);
		return ERR;
	}
	*_sockId = socket;
/*	printf ("sock if in log in func: %d \n", socket);*/
/*printf ("sock if in log in func: %d \n", _sockId);*/
	free (mngmsg);
	return OK;
}

Status CreateNewGroup (char* _userName, char* _group, int _sockId)
{
	MngMsg* mngmsg = NULL;
	mngmsg = CreateNewGroupMngMessage(_userName, _group, _sockId);
	mngmsg = SendMngMsg (mngmsg, &_sockId);
	if (strcmp(mngmsg->m_data,"ok" )!=0)
	{
		free (mngmsg);
		return ERR;
	}
	free (mngmsg);
	return OK;
}

Status JoinGroup ( char* _userName, char* _groupName ,int _sockId) 
{
	MngMsg* mngmsg = NULL;
	mngmsg = CreateJoinGroupMngMessage(_userName, _groupName ,_sockId);
	mngmsg = SendMngMsg (mngmsg ,&_sockId);
	if (strcmp(mngmsg->m_data,"ok" )!=0)
	{
		free (mngmsg);
		return ERR;
	}
	free (mngmsg);
	return OK;
}

Status StartChat (char* _groupName, char* _username,int _sockId )
{
	char bufSenderCommand[USER_NAME_SIZE];
	char bufListenerCommand[USER_NAME_SIZE];
	char* port = CHAT_PORT;
	/*char* ip;*/
	MngMsg* mngmsg = NULL;
	/*recive ip address*/
	
	mngmsg = CreateStartChatMngMessage(_username, _groupName, _sockId);
	mngmsg = SendMngMsg (mngmsg, 0);

	/*ip = mgmtMsg->m_data;*/
    pid_t pId;

	pId = fork();
	if (pId != 0)
	{
		sprintf(bufSenderCommand, "xterm -e ./SENDER %s %s %s",mngmsg->m_data, port, _username );
		system(bufSenderCommand);	 
	}
	pId = fork();
	if (pId != 0)
	{
		sprintf(bufListenerCommand, "xterm -e ./LISTENER %s %s ",mngmsg->m_data, port );
		system(bufListenerCommand);
	}
	free (mngmsg);
	return OK;
}



Status ShowAllMyGroups (char* _username ,int _sockId)
{

	MngMsg* mngmsg = NULL;
	mngmsg = CreateShowAllMyGroupsMngMessage(_username, _sockId);

	
	mngmsg = SendMngMsg (mngmsg, &_sockId);
	if (strcmp(mngmsg->m_data,"ok" )!=0)
	{
		free (mngmsg);
		return ERR;
	}
	free (mngmsg);
	return OK;




}


void GetWelcomeMsg (int _sockId)
{
	MngMsg* mngmsg = NULL;
	mngmsg = CreateWelcomeMsg();
	mngmsg= SendMngMsg(mngmsg, &_sockId);

	printf("%s\n",mngmsg->m_data);
	free (mngmsg);
}








