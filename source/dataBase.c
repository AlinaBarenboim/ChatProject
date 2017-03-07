#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "dataBase.h"
#include "../../advance_c/HashMap/HashMap.h"
#include "userDataBase.h"
#include "groupsDataBase.h"
struct DB
{
	userDB* m_userDb;
        groupDB* m_groupDb;
/*	EventDB* m_eventDb;*/
};



static int Hash(char *str)
{
	int hash = 5381;
	int c;

	while (c = *(str++))
	hash = ((hash << 5) + hash) + c; 
	return hash;
	
}

/*static int KeyEqualFunc(char* _str1, char* _str2)*/
/*{*/
/*	int result;*/
/*	result = strcmp(_str1, _str2);*/
/*	if (result == 0)*/
/*	{*/
/*		return 1;*/
/*	}*/
/*	return 0;*/
/*}*/

static int KeyEqualFunc(char* _str1, char* _str2)
{
	return (strcmp(_str1, _str2) == 0 ? 1 : 0);
}


DB* CreateDataBase (void)
{
	DB* db = NULL;
	db = (DB*)malloc(sizeof(DB));
	if (NULL == db)
	{
		printf ("error creating db \n");
		exit(EXIT_FAILURE);
	}
	db->m_userDb = CreateUserDataBase(HASH_CAPACITY,(HashFunction)Hash,(EqualityFunction)KeyEqualFunc );
	db->m_groupDb = CreateGroupDataBase(HASH_CAPACITY,(HashFunction)Hash,(EqualityFunction)KeyEqualFunc );
	return db;	
}

void DestroyDataBase (DB* _dataBase)
{
/*	DestroyUserDataBase (UserDB* _userDb);*/
/*	DestroyGroupDataBase (GroupDB* _groupDb);*/
/*	DestroyEventDataBase (EventDB* _eventDb);*/
	free (_dataBase);
}

Status InsertData(DB* _DB, int _context, MngMsg* _mngmsg)
{
	userData* userdata;
	groupData* groupdata;
	switch (_context)
	{
		case USER_DB:
			userdata = CreateUserDataPack (_mngmsg);
			if (userdata == NULL)
			{
				printf ("user data pack is null \n");
				exit (EXIT_FAILURE);
			}
/*			printf ("USER NAME in insert data: %s \n",  _mngmsg->m_userName);*/
			if (InsertDataToUserDataBase (_DB->m_userDb, _mngmsg->m_userName, userdata) != OK)
			{
				printf ("error insert user data while sign in new user \n");
				free (userdata);
				exit (EXIT_FAILURE);
			}
			
			break;
		case GROUP_DB:
			groupdata = CreateGroupDataPack (_mngmsg , _DB->m_groupDb);
			if (groupdata == NULL)
			{
				printf ("group data pack is null \n");
				exit (EXIT_FAILURE);
			}
/*			printf ("group data : name: %s, admin: %s \n", groupdata->m_groupName, groupdata->m_admin);*/
			if(InsertDataToGroupDataBase (_DB->m_groupDb, _mngmsg->m_data, (void*)groupdata) != OK)
			{
				printf ("group data insert fail \n");
				exit (EXIT_FAILURE);
			}
			
			break;
/*		case EVENT_DB:*/
/*			InserDataToEventDatabase (_DB->m_eventDb);*/
/*			break;*/
			
	}
	return OK;	
}

Status GetData(DB* _DB, int  _context, void* _key, void** _value)
{
	int result;
	switch (_context)
	{
		case USER_DB:
			result = GetDataFromUserDataBase (_DB->m_userDb, _key, _value );
			if (result == ERR)
			{
				printf ("error get data from user data base. \n");
				return ERR;
			}
			break;
		case GROUP_DB:
			if (GetDataFromGroupDataBase (_DB->m_groupDb,_key, _value) != OK)
			{
				printf ("error get data from group db \n");
				return ERR;
			}

			break;
/*		case EVENT_DB:*/
/*			GetDataFromEventDatabase (_DB->m_eventDb);*/
/*			break;*/
			
	}
	return OK;
	
}


Status UpdateData (DB* _DB,  MngMsg* _mngmsg)
{

	int result;
	switch (  _mngmsg->m_opcode)
	{
		case MNG_MSG_LOG_IN:
			result = UpdateDataInUserDataBase (_DB->m_userDb, _mngmsg);
			if (result == ERR)
			{
				return ERR;
			}
			break;
			
		case MNG_MSG_JOIN_GROUP:
			result = UpdateDataInUserDataBase (_DB->m_userDb, _mngmsg);
			if (result == ERR)
			{
				return ERR;
			}
			result = UpdateGroupInGroupDataBase (_DB->m_groupDb ,_mngmsg);
			if (result == ERR)
			{
				return ERR;
			}
			break;
/*		case GROUP_DB:*/
/*			GetDataFromGroupDatabase (_DB->m_groupDb);*/
/*			break;*/
/*		case EVENT_DB:*/
/*			GetDataFromEventDatabase (_DB->m_eventDb);*/
/*			break;*/
			
	}
	return OK;
	
}

