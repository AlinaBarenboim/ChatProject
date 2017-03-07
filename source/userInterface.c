#include <stdio.h>
#include <stdlib.h>
#include "intern.h"
#include "user.h"
#include "superUser.h"
#include "userInterface.h"
#include "mngProt.h"
#define SUPER_USER_NAME "Alina"
#define SUPER_USER_PASSWORD "alina123"
#define WRONG_KEY 0xdeadbeef
/*define NDEBUG*/
/*define NTRACE*/


struct UI
{
	char* m_super_user_name;
	char* m_super_password;
};



static void UiLevelTwoFunctionUser (char* _userName, int _sockId)
{
	int input, wrongKey = 0;
	char groupName [USER_NAME_SIZE];
	do 
	{
	
		printf ("Please choise one of the following options: \n");
	
		printf("1. Join Group \n");
		printf("2. Leave Group \n");
		printf("3.Start Chat\n");
		printf("4. Show All My Groups \n");
		printf("5. Create New Group \n");
		printf("6. Close Group \n");
		printf("7. Show All Groups \n");
		printf("8. Log Out \n");
		printf ("\n");
		scanf ("%d", &input);
		printf ("Your choise is: %d \n", input);
		switch (input)
		{
			case JOIN_GROUP:
				printf ("Please enter a group name (one word only) \n");
				scanf ("%s", groupName);
				if (JoinGroup (_userName, groupName, _sockId) != OK)
				{
					printf ("Something is wrong. Please try again. \n");
				}
				else 
				{
					printf ("Successfully added to %s group! \n", groupName);
			
				}
				wrongKey = 0;
				break;
					
			case LEAVE_GROUP:
				wrongKey = 0;
				break;
			case START_CHAT:
				printf ("Please enter a group name. \n");
				scanf ("%s", groupName);
				if (StartChat (groupName, _userName, _sockId) != OK)
				{
					printf ("Something is wrong. Please try again. \n");
				}
				
				else
				{
					printf ("Successfully joined to chat! \n");

				}
				wrongKey = 0;
				break;
			case SHOW_ALL_MY_GROUPS:
				if (ShowAllMyGroups (_userName, _sockId) != OK)
				{
					printf ("Something is wrong. Please try again! \n");
					wrongKey = 0;
					break;
				}
		
				wrongKey = 0;
				break;
			case CREATE_NEW_GROUP:

				printf ("Please choise a group name up to 10 characters(one word only): \n"); /*TODO check is group name already exist*/
				scanf ("%s", groupName);
				/*TODO check if group already exist*/
				if (CreateNewGroup( _userName, groupName, _sockId) != OK)
				{
					printf ("Something is wrong. Please try again! \n");
					wrongKey = 0;
					break;
				}
				else
				{
					printf ("Group Created Successfully! \n");
					wrongKey = 0;
					break;
				}
			

			case CLOSE_GROUP:
				wrongKey = 0;
				break;
			case SHOW_ALL_GROUPS:
				wrongKey = 0;
				break;
			case LOG_OUT:
				wrongKey = 0;
				break;
			default:
				printf("No such option. Please try again: \n");
				wrongKey = WRONG_KEY;
				break;
		}
		
	}while (wrongKey != WRONG_KEY);
/*						*/
/*								*/
}


static UI*  CreateUI () /*TODO super user from config file*/
{
	UI* ui;
	ui = (UI*)malloc(sizeof(UI));
	if (NULL == ui)
	{
		printf("malloc fail in create ui function \n");
		exit (EXIT_FAILURE);
	}
	ui->m_super_password = malloc(PASSWORD_SIZE*sizeof(char));
	if (ui->m_super_password ==NULL)
	{
		free (ui);
		printf ("Super password error \n");
		exit (EXIT_FAILURE);
		
	}
	ui->m_super_user_name = malloc(USER_NAME_SIZE*sizeof(char));
	if (ui->m_super_user_name == NULL)
	{
		free (ui->m_super_password);
		free (ui);
		printf ("Super username error \n");
		exit (EXIT_FAILURE);
		
	}
	ui->m_super_user_name = SUPER_USER_NAME;
	ui->m_super_password = SUPER_USER_PASSWORD;
	return ui;
}


static void RunUI (UI* _ui)
{
	/*TODO WELCOME FROM SERVER*/
	int input, wrongKey = 0;
	int retVal;
	int sockId ;
	MngMsg* mngmsg= NULL;
	char userName [USER_NAME_SIZE];
	char password [PASSWORD_SIZE];
/*	GetWelcomeMsg ();*/
/*	printf("WELCOME TO A&D CRAZY CHAT !\n");*/
	do 
	{
		printf("Please choise one of the following options: \n");
		printf("1. SignUp as new user \n");
		printf("2. LogIn as existing user\n");
	
		scanf ("%d",&input );

	
		switch (input)
		{
			case SIGN_UP:
				printf ("Please choise a user name up to 10 characters(one word only): \n"); /*TODO check is user name and password already exist*/
				scanf ("%s", userName);
				printf ("Please choise a password up to 10 characters (one word only): \n");
				scanf ("%s", password);
				mngmsg = UserSignUp( userName, password, 0);
				if (mngmsg == NULL)
				{
					/*TODO give an answer to user*/
					printf("Sign in error! \n");
					exit(EXIT_FAILURE);
				}
/*				UiLevelTwoFunctionUser (user);*/
				wrongKey = 0;
				break;
			
			case LOG_IN:
				do 
				{
					printf ("Please enter a user name up to 10 characters for log in(one word only): \n"); /*TODO check is user name and password already exist*/
					scanf ("%s", userName);
					if (CheckIfUserExist (userName, 0) != EXIST)
					{
						printf ("User name not exist. Try again. \n");
					}
				}
				while (CheckIfUserExist (userName, 0) != EXIST);
				
				printf ("Please enter a password up to 10 characters (one word only): \n");
				scanf ("%s", password);
	
				 if (UserLogIn ( userName, password , &sockId) != OK)
				 {
				 	printf ("Wrong password. Try again. \n");
				 	wrongKey = 0;
				 					 printf ("Sock ID received: %d \n", sockId);
				 }

				else 
				{
					 printf ("Sock ID received: %d \n", sockId);
					UiLevelTwoFunctionUser (userName, sockId);
					wrongKey = WRONG_KEY;
				}


				break;
			default:
				printf ("No Such Option. Try again. \n");
				wrongKey = WRONG_KEY;
				break;
				
		
			
		}
	}while (wrongKey != WRONG_KEY);

}



int main()
{
	UI* ui = CreateUI ();

	CreateMngProtocol (NULL);
	GetWelcomeMsg (0);
	RunUI (ui);
}
