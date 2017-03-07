#CHAT PROJECT

CFLAGS = -g -c #-ansi -pedantic -Wall -Werror -g -c
CC = gcc


UI_OBJS = userInterface.o user.o mngProt.o clientMng.o 

MNG_SERVER_OBJS= mngServer.o initializer.o clientMng.o mngProt.o dataBase.o userDataBase.o mngManager.o groupsDataBase.o


LISTENER_OBJS = listener.o
SENDER_OBJS = sender.o

.PHONY: clean rebuild 

SENDER:  $(SENDER_OBJS)
	$(CC) -o SENDER $(SENDER_OBJS)
	
LISTENER:  $(LISTENER_OBJS)
	$(CC) -o LISTENER $(LISTENER_OBJS)

UI: $(UI_OBJS)
	 $(CC) -o UI $(UI_OBJS)
	 
	 

MNG_SERVER: $(MNG_SERVER_OBJS)
	 $(CC) -o MNG_SERVER $(MNG_SERVER_OBJS) -L../../advance_c -lds -lm

#userInterface : userInterface.o
#	$(CC) $(UI_OBJS) -o userInterface $(USER_OBJS)

userInterface.o : userInterface.c userInterface.h intern.h user.h superUser.h mngProt.h
	$(CC) $(CFLAGS) -o userInterface.o userInterface.c 
	
#user : user.o
#	$(CC) $(USER_OBJS) -o user $(MNGPROT_OBJS)

user.o : user.c user.h userInterface.h intern.h user.h superUser.h mngProt.h
	$(CC) $(CFLAGS) -o user.o user.c 
	
#mngProt : mngProt.o
#	$(CC) $(MNGPROT_OBJS) -o mngProt 

#mngProt.o : mngProt.c mngProt.h  intern.h user.h superUser.h
#	$(CC) $(CFLAGS) -o mngProt.o mngProt.c 
	
#server : server.o ../../advance_c/HashMap/HashMap.h ConfigServer.h
#	$(CC) $(SRV_OBJS) -o server -L../../advance_c/ -lds -lm ConfigServer.c

mngServer.o : mngServer.c mngServer.h ../../advance_c/HashMap/HashMap.h intern.h
	$(CC) $(CFLAGS) -o mngServer.o mngServer.c 

mngProt.o : mngProt.c mngProt.h clientMng.h intern.h clientMng.h
	$(CC) $(CFLAGS) -o mngProt.o mngProt.c 
	
clientMng.o : clientMng.c  clientMng.h intern.h
	$(CC) $(CFLAGS) -o clientMng.o clientMng.c 

	
initializer.o : initializer.c  initializer.h  mngProt.h intern.h clientMng.h mngServer.h  mngProt.h
	$(CC) $(CFLAGS) -o initializer.o initializer.c 
	


dataBase.o : dataBase.c dataBase.h  groupsDataBase.h userDataBase.h  intern.h ../../advance_c/HashMap/HashMap.h
	$(CC) $(CFLAGS) -o dataBase.o dataBase.c 
	
userDataBase.o : userDataBase.c userDataBase.h dataBase.h ../../advance_c/HashMap/HashMap.h intern.h
	$(CC) $(CFLAGS) -o userDataBase.o userDataBase.c 
	
	
groupsDataBase.o : groupsDataBase.c groupsDataBase.h dataBase.h ../../advance_c/HashMap/HashMap.h intern.h
	$(CC) $(CFLAGS) -o groupsDataBase.o groupsDataBase.c 
	
mngManager.o : mngManager.c mngManager.h intern.h dataBase.h
	$(CC) $(CFLAGS) -o mngManager.o mngManager.c 
	
sender.o : sender.c 	
	$(CC) $(CFLAGS) -o sender.o sender.c 
	
listener.o : listener.c 	
	$(CC) $(CFLAGS) -o listener.o listener.c 	
	
#client : client.o ConfigClient.h ../../advance_c/HashMap/HashMap.h
#	$(CC) $(CLT_OBJS) -o client ConfigClient.c -L../../advance_c/ -lds -lm

#client.o : client.c  client.h ConfigClient.c ConfigClient.h
#	$(CC) $(CFLAGS) client.c ConfigClient.c

#ConfigClient : ConfigClient.o 
#	$(CC) $(CNFG_OBJS) -o configClient
	
#ConfigClient.o : ConfigClient.c ConfigClient.h
#	$(CC) $(CFLAGS) ConfigClient.c.
	
#ConfigServer : ConfigServer.o 
#	$(CC) $(CNFGS_OBJS) -o configServer
	
#ConfigServer.o : ConfigServer.c ConfigServer.h
#	$(CC) $(CFLAGS) ConfigServer.c.

clean:
	rm -f *.o
	rm -f UI
	rm -f MNG_SERVER
	rm -f LISTENER
	rm -f SENDER
	
rebuild: clean UI MNG_SERVER SENDER LISTENER
