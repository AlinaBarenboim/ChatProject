#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../../advance_c/HashMap/HashMap.h"
#include "groupsDataBase.h"
#include "dataBase.h"
#define MULTI_IP "224.0.0."



struct groupDB 
{
	HashMap* m_groups;
	size_t m_numOfGroups;
	char** m_Ips;
};


static void PrintGroupDataBase (char* _key, groupData* _value)
{
	int i = 0;
	printf ("GROUP KEY: %s \n" , _key);
	printf ("GROUP NAME: %s \n" , _value->m_groupName);
	printf ("GROUP ADMIN: %s \n" , _value->m_admin);
	printf ("NUMBER OF USERS IN GROUP: %d \n" , _value->m_numOfUsers);
		printf ("IP ADDRESS: %s \n" , _value->m_ipAdress);
	for (i = 0; i < _value->m_numOfUsers ; ++i)
	{
		printf ("GROUP USERS: %s \n" , _value->m_usersInGroup[i]);
	}
}



groupData* CreateGroupDataPack (MngMsg* _mngmsg, groupDB* _groupDb)
{
		groupData* groupdata;
		static int ipIndex = 0;
		groupdata = (groupData*)malloc(sizeof(groupData));
		if (NULL == groupdata)
		{
			printf ("error creating group data struct \n");
			exit (EXIT_FAILURE);
		}
		groupdata->m_usersInGroup =(char**)calloc(MAX_IN_GROUP,sizeof(char*));	
		if (NULL == groupdata->m_usersInGroup)
		{
			printf ("error creating group data struct \n");
			free (groupdata);
			exit (EXIT_FAILURE);
		}
		groupdata->m_groupName = _mngmsg->m_data;
		groupdata->m_admin = _mngmsg->m_userName;
		groupdata->m_numOfUsers = 1;
		groupdata->m_usersInGroup[0] = _mngmsg->m_userName;
		groupdata->m_ipAdress = (char*)malloc(USER_NAME_SIZE*sizeof(char));
		strcpy(groupdata->m_ipAdress, _groupDb->m_Ips[ipIndex]);
		groupdata->m_ipAdress = _groupDb->m_Ips[ipIndex];
		ipIndex ++;
		return groupdata;
}



groupDB* CreateGroupDataBase (size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc)
{
	groupDB* groupDataBase;
	int i = 0;
		int mask = 0;
		char dest [USER_NAME_SIZE];
		char* ipAdd = (char*)malloc(USER_NAME_SIZE*sizeof(char));
		
		int j;
		ipAdd = MULTI_IP;
		char ipMask[USER_NAME_SIZE];
	groupDataBase = (groupDB*)malloc(sizeof(groupDB));
	if (NULL == groupDataBase)
	{
		printf ("error creating group db 1 \n");
		exit (EXIT_FAILURE);
	}
	
	groupDataBase->m_groups= HashMap_Create( _capacity, _hashFunc,  _keysEqualFunc);
	if (NULL == groupDataBase->m_groups)
	{
		printf ("error creating group db 2 \n");
		free (groupDataBase);
		exit (EXIT_FAILURE);
	}
	groupDataBase->m_Ips = (char**)malloc(MAX_GROUPS_NUM*sizeof(char*));
	if (NULL == groupDataBase->m_Ips)
	{
		printf ("error creating group db 3 \n");
		free ( groupDataBase->m_groups);
		free (groupDataBase);
		exit(EXIT_FAILURE);
	}
	
	for (i=0; i< MAX_GROUPS_NUM; i++)
	{
	
		groupDataBase->m_Ips[i] = malloc(USER_NAME_SIZE*sizeof(char));
		if (NULL == groupDataBase->m_Ips[i])
		{
			printf ("ERR MALLOC IP . \n");
			exit (EXIT_FAILURE);
		}		
	}		
	for (i=0 ; i< MAX_GROUPS_NUM ;++i)
	{
		mask ++;
		memset(dest, 0, sizeof(dest));
		strcpy (dest, ipAdd);
		sprintf (ipMask,"%d", mask);
		strcat(dest , ipMask);
		strcpy(	groupDataBase->m_Ips[i], dest);
		printf("Available IPs: %s \n", groupDataBase->m_Ips[i]);
	}
	groupDataBase->m_numOfGroups = 0;
	return groupDataBase;	
}

Status InsertDataToGroupDataBase (groupDB* _groupDb, void* _key, void* _value)
{
	if (HashMap_Insert(_groupDb->m_groups, _key, _value) != MAP_SUCCESS)
	{
		printf("Can't add a group %s into group data base \n",(char*) _key);
		return ERR;
	}
	_groupDb->m_numOfGroups++;
	printf ("New group with key %s   just inserted into group data base\n", (char*) _key /*(groupData*)_value->m_groupName*/);
	printf ("Current groups number: %lu \n", _groupDb->m_numOfGroups);
/*	HashMap_ForEach(_groupDb->m_groups, (KeyValueActionFunction)PrintGroupDataBase,NULL);*/
	return OK;
}

Status GetDataFromGroupDataBase (groupDB* _groupDb, void* _key, void** _value)
{
	if (HashMap_Find(_groupDb->m_groups,(char*)_key, _value) != MAP_SUCCESS)
	{
		printf("Can't find a group %s in group data base \n",(char*) _key);
		return ERR;
	}
	return OK;	
}

Status RemoveDataFromGroupDataBase (groupDB* _groupDb, void* _key)
{
	void* value;
	if (HashMap_Remove(_groupDb->m_groups, _key, &value) != MAP_SUCCESS)
	{
		printf("Can't remove a group %s from group data base \n", (char*)_key);
		return ERR;
	}
	_groupDb->m_numOfGroups--;
	return OK;	
}


Status UpdateGroupInGroupDataBase (groupDB* _groupDb ,MngMsg* _mngmsg)
{
	groupData* value;
	int counter = 0;
	int index = 0;
	if (HashMap_Remove(_groupDb->m_groups, _mngmsg->m_data, (void**)&value) != MAP_SUCCESS)
	{
		printf("Can't remove a group %s from group data base. Update fail. \n", _mngmsg->m_data);
		return ERR;
	}
	switch (_mngmsg->m_opcode)
	{
		case MNG_MSG_JOIN_GROUP:
		for (index = 0; index < MAX_IN_GROUP; ++index)
		{
			if (value->m_usersInGroup[index] == 0)
			{
				value->m_usersInGroup[index] = _mngmsg->m_userName;
				value->m_numOfUsers++;
				break;

			}
			else
			{
				counter++;
			}
		
		}
		if (counter >= MAX_IN_GROUP)
		{
			printf ("Error while join group. Too much users in group. \n");
			return ERR;
		}
		break;

	}	
	if (HashMap_Insert(_groupDb->m_groups, _mngmsg->m_data, value) != MAP_SUCCESS)
	{
		printf("Can't add  group %s to group data base. Update fail. \n",_mngmsg->m_data);
		return ERR;
	}
	PrintGroupDataBase (_mngmsg->m_data,value);
	return OK;	
}


/*TODO destroy function*/
