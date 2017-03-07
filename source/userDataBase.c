#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../../advance_c/HashMap/HashMap.h"
#include "userDataBase.h"
#include "dataBase.h"


struct userDB 
{
	HashMap* m_users;
	size_t m_numOfUsers;
/*	HashFunction m_hashFunction;*/
/*	EqualityFunction m_keysEqualFunction;*/
};


static void PrintUserDataBase (char* _key, userData* _value)
{
	int i;
	
	printf ("PRINTING USER INFORMATION: \n");
	printf ("User Key: %s\n", _key);
	printf ("UserName: %s\n", _value->m_userName);
	printf ("Password: %s\n", _value->m_password);
	printf ("Soket Id: %d\n", _value->m_socketId);
	printf ("isAdmin: %d\n", _value->m_isAdmin);
	printf ("isBanned: %d\n", _value->m_isBanned);
	printf ("isOnline: %d\n", _value->m_isOnline);
	printf ("num of groups: %d\n", _value->m_numOfGroups);
	for (i = 0; i < _value->m_numOfGroups; ++i)
	{
		printf ("group: %s \n", _value->m_groups[i]);
	}

}



userData* CreateUserDataPack (MngMsg* _mngmsg)
{
		userData* userdata;
		int i;
/*		printf ("in CREATE DATA PACK _mngmsg->m_userName : %s \n", _mngmsg->m_userName);*/
		userdata = (userData*)malloc(sizeof(userData));
		if (NULL == userdata)
		{
			printf ("error creating user data struct \n");
			exit (EXIT_FAILURE);
		}
			
		userdata->m_userName = malloc (USER_NAME_SIZE*sizeof(char));
		if (NULL == userdata->m_userName)
		{
			printf ("USERNAME MALLOC NULL \n");
			free (userdata);
			exit (EXIT_FAILURE);
		}
		
		userdata->m_password = malloc (USER_NAME_SIZE*sizeof(char));
		if (NULL == userdata->m_password)
		{
			printf ("USER PASSWORD MALLOC NULL \n");
			free( userdata->m_userName);
			free (userdata);
			exit (EXIT_FAILURE);
		}
		userdata->m_userName = _mngmsg->m_userName;
/*		printf ("user name in create data pack: %s \n",userdata->m_userName);*/
		userdata->m_password = _mngmsg->m_password;
		userdata->m_isAdmin = NOT_ADMIN;
		userdata->m_isBanned = NOT_BANNED;
		userdata->m_isOnline = OFFLINE;
		userdata->m_groups = (char**)calloc(MAX_GROUPS_NUM,sizeof(char*));
		if (NULL == userdata->m_groups)
		{
			printf ("error creating userdata groups malloc \n");
			free (userdata);
			exit(EXIT_FAILURE);
		}
		
		for (i=0; i< MAX_GROUPS_NUM; i++)
		{
	
			userdata->m_groups[i] = calloc(USER_NAME_SIZE,sizeof(char));
			if (NULL == userdata->m_groups[i])
			{
				printf ("ERR MALLOC GROUP . \n");
				exit (EXIT_FAILURE);
			}	
		
		}
		userdata->m_socketId = _mngmsg->m_socketId;
		userdata->m_numOfGroups = 0;
		return userdata;
}





userDB* CreateUserDataBase (size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc)
{
	userDB* userDataBase;
	userDataBase = (userDB*)malloc(sizeof(userDB));
	if (NULL == userDataBase)
	{
		printf ("error creating user db 1 \n");
		exit (EXIT_FAILURE);
	}
	

	userDataBase->m_users = HashMap_Create( _capacity, _hashFunc,  _keysEqualFunc);
/*	printf ("user data base create: %p \n", userDataBase->m_users);*/
	if (NULL == userDataBase->m_users)
	{
		printf ("error creating user db 2 \n");
		free (userDataBase);
		exit (EXIT_FAILURE);
	}

	userDataBase->m_numOfUsers = 0;
	return userDataBase;	
}

Status InsertDataToUserDataBase (userDB* _userDb, void* _key, void* _value)
{
	size_t size;
	if (HashMap_Insert(_userDb->m_users, _key, _value) != MAP_SUCCESS)
	{
		printf("error insert into user hash map \n");
		exit (EXIT_FAILURE);
	}
	_userDb->m_numOfUsers++;
/*	printf ("NUMBER OF USERS IN HASHMAP: %d", _userDb->m_numOfUsers);*/
	
	PrintUserDataBase (_key, _value);
	return OK;
}

Status GetDataFromUserDataBase (userDB* _userDb, void* _key, void** _value)
{

/*	printf ("Key in user db func: %s ", (char*)_key);*/
	if (HashMap_Find(_userDb->m_users,_key, _value) != MAP_SUCCESS)
	{
		printf("error find in user hash map \n");
		return ERR;
	}
	return OK;	
}

Status RemoveDataFromUserDataBase (userDB* _userDb, void* _key)
{
	userData* value;
	value = (userData*)malloc(sizeof(userData));
	if (NULL == value)
	{
		printf("error value malloc \n");
		exit(EXIT_FAILURE);
	}
	if (HashMap_Remove(_userDb->m_users, _key, (void**)&value) != MAP_SUCCESS)
	{
		printf("error remove from user hash map \n");
		exit (EXIT_FAILURE);
	}
	_userDb->m_numOfUsers--;
	return OK;	
}

/*Status UpdateDataInUserDataBase (userDB* _userDb ,MngMsg* _mngmsg)*/
/*{*/
/*	userData* value;*/
/*	int i;*/
/*	if (HashMap_Remove(_userDb->m_users, _mngmsg->m_userName,(void**) &value) != MAP_SUCCESS)*/
/*	{*/
/*		printf("error remove from user hash map \n");*/
/*		exit (EXIT_FAILURE);*/
/*	}*/
/*	printf ("Following user information removed: \n");*/
/*	PrintUserDataBase (_mngmsg->m_userName, (void*)value);*/
/*	switch (_mngmsg->m_opcode)*/
/*	{*/
/*		case MNG_MSG_LOG_IN:*/
/*			*/
/*			value->m_isOnline = ONLINE;*/
/*			if (HashMap_Insert(_userDb->m_users, _mngmsg->m_userName, value) != MAP_SUCCESS)*/
/*			{*/
/*				printf("error remove from user hash map \n");*/
/*				return ERR;*/
/*			}*/
/*			break;*/
/*		*/
/*		case MNG_MSG_JOIN_GROUP:*/
/*			for(i=0; i < MAX_GROUPS_NUM; ++i)*/
/*			{*/
/*				if (value->m_groups[i] == 0)*/
/*				{*/
/*					value->m_groups[i] = _mngmsg->m_data;*/
/*					if (HashMap_Insert(_userDb->m_users, _mngmsg->m_userName, value) != MAP_SUCCESS)*/
/*					{*/
/*						printf("ERROR insert to user hash map after update \n");*/
/*						return ERR;*/
/*					}*/
/*				}*/
/*				else*/
/*				{*/
/*					return ERR;*/
/*				}*/
/*			}	*/

/*			break;*/
/*			*/
/*	}*/
/*	printf ("Following user information inserted: \n");*/
/*	PrintUserDataBase (_mngmsg->m_userName, (void*)value);*/
/*	return OK;	*/
/*}*/


Status UpdateDataInUserDataBase (userDB* _userDb ,MngMsg* _mngmsg)
{
	userData* value;
	int index = 0;
	int counter;
	size_t size;
	value = (userData*)malloc(sizeof(userData));
	if (NULL == value)
	{
		printf("error value malloc \n");
		exit(EXIT_FAILURE);
	}
	
	switch (_mngmsg->m_opcode)
	{
		case MNG_MSG_LOG_IN:
			PrintUserDataBase (_mngmsg->m_userName, (void*)value);
			if (HashMap_Remove(_userDb->m_users, _mngmsg->m_userName, (void**)&value) != MAP_SUCCESS)
			{
				
				printf("error remove  user %s from user hash map \n",_mngmsg->m_userName );
				return ERR;
			}
			printf ("JUST REMOVED USER : %s FROM DB \n", _mngmsg->m_userName);
			value->m_isOnline = ONLINE;
			if (HashMap_Insert(_userDb->m_users, _mngmsg->m_userName, value) != MAP_SUCCESS)
			{
				printf("error insert  user %s to user  hash map after update \n", _mngmsg->m_userName);
				return ERR;
			}
			printf ("JUST INSERT BACK USER : %s TO DB \n", _mngmsg->m_userName);
			PrintUserDataBase (_mngmsg->m_userName, (void*)value);
		break;
		
		case MNG_MSG_JOIN_GROUP:
			if (HashMap_Remove(_userDb->m_users, _mngmsg->m_userName, (void**)&value) != MAP_SUCCESS)
			{
				printf("error remove  user %s from user hash map \n",_mngmsg->m_userName );
				return ERR;
			}
			printf ("JUST REMOVED USER : %s FROM DB \n", _mngmsg->m_userName);
			value->m_numOfGroups ++;
			value->m_groups[value->m_numOfGroups-1] = _mngmsg->m_data;
			PrintUserDataBase (_mngmsg->m_userName, (void*)value);
			if (HashMap_Insert(_userDb->m_users, _mngmsg->m_userName, value) != MAP_SUCCESS)
			{
				printf("error insert  user %s to user  hash map after update \n", _mngmsg->m_userName);
				return ERR;
			}
			printf ("JUST INSERT BACK USER : %s TO DB \n", _mngmsg->m_userName);
			PrintUserDataBase (_mngmsg->m_userName, (void*)value);
			break;

	}

	return OK;	


}

/*TODO destroy function*/
