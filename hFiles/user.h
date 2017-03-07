#ifndef _USER_H_
#define _USER_H_
#include "intern.h"




/*Status CreateUser (const char* _userName, const char* _password);
void DestroyUser(User* _user);*/


void GetWelcomeMsg (int _sockId);

MngMsg* UserSignUp  (char* _userName, char* _password, int _sockId);
int CheckIfUserExist (char* _userName, int _sockId);
Status JoinGroup (char* _userName, char* _groupName, int _sockId);

Status StartChat (char* _groupName, char* _username, int _sockId);
Status ShowAllMyGroups (char* _username, int _sockId);




Status UserLogIn (  char* _userName, char* _passwor, int* _sockId);
int UserLogOut (MngMsg* _user , int _sockId);



int LeaveGroup (MngMsg* _user, const char* _group);
int SendMSG (MngMsg* _user, const char* _group, const char* _msg);



Status CreateNewGroup (char* _userName, char* _group, int _sockId);
int CloseGroup (MngMsg* _user, const char* _group);
int ShowAllGroups();

;
#endif
