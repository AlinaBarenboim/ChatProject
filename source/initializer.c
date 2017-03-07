#include "mngProt.h"
#include <stdio.h>
#include <stdlib.h>
#include "clientMng.h"
/*#define PORT 1291*/
/*#define IP "127.0.0.1"*/
#include "intern.h"
#define BUFF_SIZE 1024
/*define NDEBUG*/
/*define NTRACE*/
#include "mngServer.h"
#define MAX_NUM_OF_CLIENTS 100
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include "mngManager.h"



static void replayFunction (int _socketNum, char* _payload)
{
/*	printf ("payload in replay func: %s \n", _payload);*/
	int retVal;
	int len;
	int i;
	MngMsg* mngmsg;
	char* ip;
	char* reply;
	char* tempReply;
	char** groups;
	mngmsg = StringToStract(_payload);
	reply = malloc(USER_NAME_SIZE*sizeof(char));
	if (NULL == reply)
	{
		printf ("reply malloc null \n");
		exit (EXIT_FAILURE);
	}
	
	switch (mngmsg->m_opcode)
	{
		case MNG_MSG_SIGN_UP:
			reply = " | | | |ok|";
			len = strlen (reply)+1;
			if (ExecuteUserSingIn ( mngmsg) != OK)
			{
				printf ("error execute user sign in \n");
				exit(EXIT_FAILURE);
			}
			retVal = send(_socketNum, reply, len , MSG_NOSIGNAL);
			if (retVal == 0)
			{
				perror ("write return 0: \n");
				exit (EXIT_FAILURE);
			}
			if (retVal == -1)
			{
				perror("write fail : \n");
				exit (EXIT_FAILURE);
			}

			break;
		
		case MNG_MSG_CHECK_IF_USER_EXIST:
			if( ExecuteCheckIfUserExist( mngmsg) != OK)
			{
				reply = " | | | |error|";
				len = strlen (reply)+1;
				retVal = send(_socketNum, reply, len , MSG_NOSIGNAL);
				if (retVal == 0)
				{
					perror ("write return 0: \n");
					exit (EXIT_FAILURE);
				}
				if (retVal == -1)
				{
					perror("write fail : \n");
					exit (EXIT_FAILURE);
				}
			}
			else 
			{
				reply = " | | | |ok|";
				len = strlen (reply)+1;
				retVal = send(_socketNum, reply, len , MSG_NOSIGNAL);
				if (retVal == 0)
				{
					perror ("write return 0: \n");
					exit (EXIT_FAILURE);
				}
				if (retVal == -1)
				{
					perror("write fail : \n");
					exit (EXIT_FAILURE);
				}
			}
		break;
		
		case MNG_MSG_LOG_IN:
			if (ExecuteUserLogIn( mngmsg) != OK)
			{
				printf ("error execute user log in \n");
				reply = " | | | |error|";
				len = strlen (reply)+1;
				retVal = send(_socketNum, reply, len , MSG_NOSIGNAL);
				if (retVal == 0)
				{
					perror ("write return 0: \n");
					exit (EXIT_FAILURE);
				}
				if (retVal == -1)
				{
					perror("write fail : \n");
					exit (EXIT_FAILURE);
				}
			}
			else 
			{
/*				sprintf(reply,"%d| | | |ok|", _socketNum);*/
				sprintf(reply,"*|*|*|*|ok|", _socketNum);
				len = strlen (reply)+1;
				retVal = send(_socketNum, reply, len , MSG_NOSIGNAL);
				if (retVal == 0)
				{
					perror ("write return 0: \n");
					exit (EXIT_FAILURE);
				}
				if (retVal == -1)
				{
					perror("write fail : \n");
					exit (EXIT_FAILURE);
				}
			}
			break;
			
			case MNG_MSG_CREATE_GROUP:
			
			if (ExecuteCreateNewGroup( mngmsg) != OK)
			{
				printf ("error creating new group \n");
				reply = " | | | |error|";
				len = strlen (reply)+1;
				retVal = send(_socketNum, reply, len , MSG_NOSIGNAL);
				if (retVal == 0)
				{
					perror ("write return 0: \n");
					exit (EXIT_FAILURE);
				}
				if (retVal == -1)
				{
					perror("write fail : \n");
					exit (EXIT_FAILURE);
				}
			}
			else 
			{
				reply = " | | | |ok|";
				len = strlen (reply)+1;
				retVal = send(_socketNum, reply, len , MSG_NOSIGNAL);
				if (retVal == 0)
				{
					perror ("write return 0: \n");
					exit (EXIT_FAILURE);
				}
				if (retVal == -1)
				{
					perror("write fail : \n");
					exit (EXIT_FAILURE);
				}
			}
			break;
			
			case MNG_MSG_JOIN_GROUP:
			if (ExecuteJoinGroup (mngmsg) != OK)
			{
				printf ("error join  group \n");
				reply = "*|*|*|*|error|";
				len = strlen (reply)+1;
				retVal = send(_socketNum, reply, len , MSG_NOSIGNAL);
				if (retVal == 0)
				{
					perror ("write return 0: \n");
					exit (EXIT_FAILURE);
				}
				if (retVal == -1)
				{
					perror("write fail : \n");
					exit (EXIT_FAILURE);
				}
			}
			else 
			{
				reply = "*|*|*|*|ok|";
				len = strlen (reply)+1;
				retVal = send(_socketNum, reply, len , MSG_NOSIGNAL);
				if (retVal == 0)
				{
					perror ("write return 0: \n");
					exit (EXIT_FAILURE);
				}
				if (retVal == -1)
				{
					perror("write fail : \n");
					exit (EXIT_FAILURE);
				}
			}
			break;
			case MNG_MSG_START_CHAT:
				ip =  ExecuteFindGroupInfo (mngmsg) ;
				
				if (strcmp (ip, "error")== 0)
				{
					strcpy(ip, "Error");
				}
				
				printf ("IP in initializer: %s \n" , ip);
				sprintf(reply, "*|*|*|*|%s|", ip);
				len = strlen (reply)+1;
				retVal = send(_socketNum, reply, len , MSG_NOSIGNAL);
				if (retVal == 0)
				{
					perror ("write return 0: \n");
					exit (EXIT_FAILURE);
				}
				if (retVal == -1)
				{
					perror("write fail : \n");
					exit (EXIT_FAILURE);
				}
				break;
				
					
		case MNG_MSG_WELCOME:
			reply = "*|*|*|*|Welcome to A&D chat!|";	
			len = strlen(reply) + 1;
			
			#ifndef NDEBUG
			printf("server sending welcome message..\n");		
			#endif
			
			retVal = send( _socketNum ,(const void*) reply, len , MSG_NOSIGNAL );
			if (retVal < 0)
			{
				perror("send faild\n");
				exit(EXIT_FAILURE);
			}
			break;	
				
			case MNG_MSG_SHOW_ALL_MY_GROUPS:
			

			groups = (char**)malloc(MAX_GROUPS_NUM*sizeof(char*));
			if (NULL == groups)
			{
				printf("malloc groups error \n");
				exit (EXIT_FAILURE);
				
			}
			for (i = 0; i < USER_NAME_SIZE; ++i)
			{
				groups[i] = malloc(USER_NAME_SIZE*sizeof(char));
				if (NULL == groups[i])
				{
					printf("malloc groups error \n");
					free (groups);
					exit (EXIT_FAILURE);
				
				}
			}
			
				if (ExecuteShowAllMyGroups (mngmsg,groups ) != OK)/*FIXME*/
				{
					printf ("error show all my groups \n");
					reply = "*|*|*|*|error|";
					len = strlen (reply)+1;
					retVal = send(_socketNum, reply, len , MSG_NOSIGNAL);
					if (retVal == 0)
					{
						perror ("write return 0: \n");
						exit (EXIT_FAILURE);
					}
					if (retVal == -1)
					{
						perror("write fail : \n");
						exit (EXIT_FAILURE);
					}
				}
				else 
				{
					reply = "*|*|*|*|ok|";
					len = strlen (reply)+1;
					retVal = send(_socketNum, reply, len , MSG_NOSIGNAL);
					if (retVal == 0)
					{
						perror ("write return 0: \n");
						exit (EXIT_FAILURE);
					}
					if (retVal == -1)
					{
						perror("write fail : \n");
						exit (EXIT_FAILURE);
					}
				} 
				break;
			}
	 	}
		



void InitSystem (/*Configs* _configs*/)
{
	Server_t* server;
		server = ServerCreate(PORT, IP,BUFF_SIZE,  (CallBackFunc) replayFunction);
	CreateMngManager ();  

	ServerRun(server);
	ClientsCreate (MAX_NUM_OF_CLIENTS, PORT, IP);
	return ;	
}



int main ()
{
	InitSystem (/*Configs* _configs*/);
	return 0;
}
