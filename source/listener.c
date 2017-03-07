#include <sys/types.h>  /* for type definitions */
#include <sys/socket.h> /* for socket API calls */
#include <netinet/in.h> /* for address structs */
#include <arpa/inet.h>  /* for sockaddr_in */
#include <stdio.h>      /* for printf() and fprintf() */
#include <stdlib.h>     /* for atoi() */
#include <string.h>     /* for strlen() */
#include <unistd.h>     /* for close() */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_LEN  1024    /* maximum string size to send */
#define FAIL 0			/* program ended because of error */
#define SUCCESS 1	    /* program ended successfully */
#define RUNNING 1       /*for endess loop*/
#define ERROR(res) ((res) < (0))	   
#define NAME_LENGTH 16
#define PERMISSION_MSGQ 0644
#define CLOSE_FROM_MAIN 7
#define TEXT_LENGTH 64


/*********************************************************************/
/*                              COLORS                               */
/*********************************************************************/

#define red   "\033[0;31m"        /*  ;  31 -> red */
#define cyan  "\033[0;36m"        /* 1 -> bold ;  36 -> cyan */
#define green "\033[0;32m"        /*   32 -> green */
#define blue  "\033[0;34m"        /*   34 -> blue */
#define magenta  "\033[0;35m"
#define gray  "\033[0;37m"
#define brown  "\033[0;33m"

#define Lred   "\033[1;31m"        /*  ;  31 -> red */
#define Lcyan  "\033[1;36m"        /* 1 -> bold ;  36 -> cyan */
#define Lgreen "\033[1;32m"        /*   32 -> green */
#define Lblue  "\033[1;34m"        /*   34 -> blue */
#define Lmagenta  "\033[1;35m"
#define Lgray  "\033[1;37m"
#define yellow  "\033[1;33m"
 
#define none   "\033[0m"        /* to flush the previous property */
#define NUM_COLORS 14
static char* colors[] = {red, blue, green, cyan, magenta, gray, Lred, Lblue, Lgreen, Lcyan, Lmagenta, Lgray, brown, yellow};

/*********************************************************************/
/*                              MAIN                                 */
/*********************************************************************/


int main(int argc, char *argv[]) 
{    
	struct sockaddr_in addr;   
	struct ip_mreq mc_req;        
	struct sockaddr_in fromAddr; 
	int sock;                     
	int flag_on = 1;              
	char recvStr[MAX_LEN+1];   /*before parsing*/ 
	char recievedMsg[MAX_LEN]; /*after parsing*/   
	int recvLen;                 
	char* ipAdd;            
	unsigned short port;      
	unsigned int fromLen;        
	int res;					 
    char name[MAX_LEN+NAME_LENGTH];   
    int color;	


	
	if (argc != 3) 
	{
		return FAIL;
	}

	ipAdd = argv[1];      /* arg 1: multicast ip address */
	port = atoi(argv[2]);    /* arg 2: multicast port number */ 
	

	printf ("numip: %s \n", ipAdd);

	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(ERROR(sock)) 
	{
		perror("socket() failed");
		return FAIL;
	}
  
	res = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag_on, sizeof(flag_on));
	if(ERROR(res)) 
	{
		perror("setsockopt() failed");
		return FAIL;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family      = AF_INET;
/*	addr.sin_addr.s_addr = htonl(INADDR_ANY);*/
	addr.sin_addr.s_addr = inet_addr(ipAdd);
	
	
	addr.sin_port        = htons(port);

	res = bind(sock, (struct sockaddr *) &addr, sizeof(addr));
	if(ERROR(res)) 
	{
		perror("bind() failed");
		return FAIL;
	}

	mc_req.imr_multiaddr.s_addr = inet_addr(ipAdd);
	mc_req.imr_interface.s_addr = htonl(INADDR_ANY);

	res = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*) &mc_req, sizeof(mc_req));
	if(ERROR(res))
	{
		perror("setsockopt() failed");
		return FAIL;
	}


  	while(RUNNING) 
  	{        	
		memset(recvStr, 0, sizeof(MAX_LEN));
		memset(recievedMsg, 0, sizeof(MAX_LEN));
		fromLen = sizeof(fromAddr);
		memset(&fromAddr, 0, fromLen);

		recvLen = recvfrom(sock, recvStr, MAX_LEN, 0, (struct sockaddr*)&fromAddr, &fromLen);
		sscanf(recvStr, "%[a-z,A-Z,0-9,.,-,_]|%[a-z,A-Z,0-9,.,-,_,!,@,#,$,%,^,&,*,(,), ,?,',<,>,:,;,`,~,-,+,_,=]", name, recievedMsg);
	
		if (ERROR(recvLen)) 
		{
			perror("recvfrom() failed");
			return FAIL;
		}
        
        color = ((name[0]+name[1]-'A')/2) % NUM_COLORS;
       
    	printf("%s%s:%s %s\n", colors[color], name, none, recievedMsg);
  	} 
}

