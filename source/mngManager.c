#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


#include "dataBase.h"

struct MngManager /*FIXME*/
{
	DB* m_dataBase;
};
MngManager* mngManager;
	
void CreateMngManager ()
{

	mngManager = (MngManager*)malloc(sizeof(MngManager));
	if (NULL == mngManager)
	{
		printf ("error creating mng manager \n");
		exit (EXIT_FAILURE);
	}
	mngManager->m_dataBase = CreateDataBase (); 
	if (NULL == mngManager->m_dataBase)
	{
		printf ("error creating mngManager->m_dataBase \n");
		free (mngManager);
		exit (EXIT_FAILURE);
	}
	return ;
}

Status ExecuteUserSingIn ( MngMsg* _mngmsg)

{
	if (InsertData(mngManager->m_dataBase, USER_DB, _mngmsg) != OK)
	{
		printf ("error insert user data from mng manager \n");
		exit (EXIT_FAILURE);
	}
	return OK;
	
}


Status ExecuteCheckIfUserExist( MngMsg* _mngmsg)
{
	void* value;
	if (GetData(mngManager->m_dataBase, USER_DB, (void*)_mngmsg->m_userName, &value) != OK)
	{
		printf ("error get user data. User not exist. \n");
		return ERR;
	}
	
	return OK;
}


Status ExecuteUserLogIn (MngMsg* _mngmsg)
{
	userData* userdata;
	GetData(mngManager->m_dataBase, USER_DB, (void*)_mngmsg->m_userName, (void**)&userdata);
	
	if (userdata->m_password != _mngmsg->m_password)
	{
		printf ("wrong password! \n");
		return ERR;
	}
	
	UpdateData (mngManager->m_dataBase,  _mngmsg);  
	
	return OK;
}

Status ExecuteCreateNewGroup (MngMsg* _mngmsg)
{

	if (InsertData(mngManager->m_dataBase, GROUP_DB, _mngmsg) != OK)
	{
		printf ("error insert GROUP data from mng manager \n");
		exit (EXIT_FAILURE);
	}
	return OK;

}


Status ExecuteJoinGroup (MngMsg* _mngmsg)
{
		if (UpdateData (mngManager->m_dataBase,  _mngmsg) != OK)
		{
			printf ("error in update user data while join group. \n");
			return ERR;
		}
		return OK;
}	


char* ExecuteFindGroupInfo (MngMsg* _mngmsg)
{

	groupData* value;
	char* ip;
 	if (GetData(mngManager->m_dataBase,GROUP_DB, (void*) _mngmsg->m_data, (void**)&value) != OK)
 	{
	 	strcpy(ip, "error");

 	}
 	else 
 	{
 		ip = value->m_ipAdress;
 	}
/* 	printf ("IP: %s \n", ip);*/
 	return ip;



}

Status ExecuteShowAllMyGroups (MngMsg* _mngmsg, char** _groups)
{
		userData* userdata;
		int num, i;
		userdata = (userData*)malloc(sizeof(userData));
		if (NULL == userdata)
		{
			printf ("user data malloc fail \n");
			exit (EXIT_FAILURE);
		}
		if(GetData(mngManager->m_dataBase, USER_DB, _mngmsg->m_userName, (void**)userdata) != OK)
		{
			printf ("error in get user data while showing all my  groups. \n");
			return ERR;
		}
		num = userdata->m_numOfGroups;
	
		printf ("You are a member of  following groups: \n ");
		
		for (i = 0; i < num; ++i)
		{
			printf ("Group  %s \n", userdata->m_groups[i]);
			
		}
		memcpy(_groups,userdata->m_groups, sizeof(userdata->m_groups));
		return OK;
}


	

