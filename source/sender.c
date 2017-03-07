#include <sys/types.h>   /* for type definitions */
#include <sys/socket.h>  /* for socket API function calls */
#include <netinet/in.h>  /* for address structs */
#include <arpa/inet.h>   /* for sockaddr_in */
#include <stdio.h>       /* for printf() */
#include <stdlib.h>      /* for atoi() */
#include <string.h>      /* for strlen() */
#include <unistd.h>      /* for close() */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h> /*catch ^c*/

#define MAX_LEN  1024   /* maximum string size to send */
#define FAIL 0			/* program ended because of error */
#define SUCCESS 1	    /* program ended successfully */
#define OFF 0           /*while stop sgin is off run the loop*/
#define ON 1            /*exit if stop signis on*/
#define ERROR(res) ((res) < (0))	
#define NAME_LENGTH 16
#define PERMISSION_MSGQ 0660
#define ERROR(res) ((res) < (0))
#define CLOSE_FROM_MAIN 7
#define TEXT_LENGTH 64



static void HandlerSIGINT(int _sigNumber, siginfo_t* _infoSig, void* _context);
/*********************************************************************/
/*                              COLORS                               */
/*********************************************************************/

#define red   "\033[0;31m"        /* 0 -> normal ;  31 -> red */
#define cyan  "\033[1;36m"        /* 1 -> bold ;  36 -> cyan */
#define green "\033[4;32m"        /* 4 -> underline ;  32 -> green */
#define blue  "\033[9;34m"        /* 9 -> strike ;  34 -> blue */ 
#define black  "\033[0;30m"
#define brown  "\033[0;33m"
#define magenta  "\033[0;35m"
#define gray  "\033[0;37m" 
#define none   "\033[0m"        /* to flush the previous property */

static int s_stop = OFF;

/*********************************************************************/
/*                              MAIN                                 */
/*********************************************************************/

int main(int argc, char *argv[]) {

	int sock;                   
	char getSendString[MAX_LEN];     
	char msgToSend[MAX_LEN+NAME_LENGTH];    
	char name[NAME_LENGTH];
	struct sockaddr_in addr; 
	unsigned int sendLen;      
	char* ipAdd;          
	unsigned short port;     
	unsigned char ttl=1;     
	int res;					
    struct sigaction action;  

	
	if (argc != 4) 
	{
		return FAIL;
	}
		
	
	
	action.sa_handler = NULL;
    action.sa_sigaction = HandlerSIGINT;
	action.sa_flags = SA_SIGINFO;
	
	sigaction(2, &action, NULL);

    
	ipAdd = argv[1];       /* arg 1: multicast IP address */
	port     = atoi(argv[2]); /* arg 2: multicast port number */
    strcpy(name, (argv[3])) ;   /* arg 3: name of user */      
	
	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(ERROR(sock)) 
	{
		perror("socket() failed");
		return FAIL;
	}
	
	res = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*) &ttl, sizeof(ttl));
	if (ERROR(res)) 
	{
		perror("setsockopt() failed");
		return FAIL;
	} 
  
	memset(&addr, 0, sizeof(addr));
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ipAdd);
	addr.sin_port        = htons(port);

  	printf("Begin typing, (ctrl-C to quit):\n");

	memset(getSendString, 0, sizeof(getSendString));
	memset(msgToSend, 0, sizeof(msgToSend));
	
	while(OFF == s_stop)
	{                        
		fgets(getSendString, MAX_LEN, stdin);
		snprintf(msgToSend, MAX_LEN+NAME_LENGTH, "%s|%s", name, getSendString);
		sendLen = strlen(msgToSend);
	
		res = sendto(sock, msgToSend, sendLen, 0, (struct sockaddr *) &addr, sizeof(addr));
		if(res != sendLen) 
		{
			perror("sendto() sent incorrect number of bytes");
			return FAIL;
		}
		
  	}
	
	printf("%sShutting down...%s", cyan, none);
	sleep(1);
	
  	close(sock);  
	return SUCCESS;
}

static void HandlerSIGINT(int _sigNumber, siginfo_t* _infoSig, void* _context)
{
	s_stop = ON;
}
