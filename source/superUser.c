#ifndef _SUPER_USER_H_
#define _SUPER_USER_H_
#include "intern.h"





SuperUser* CreateSuperUser (const char* _userName, const char* _password);
void DestroySuperUser(SuperUser _superUser);

int SuperLogIn (SuperUser* _user , const char* _password);
int SuperLogOut (SuperUser _superUser);

int SuperSendMsgToAllGroups(SuperUser* _user, const char* _msg);
int SuperSendMSG (SuperUser* _user, const char* _group, const char* _msg);
int ShowAllGroups (SuperUser* _user);
int SuperCreateNewGroup (SuperUser* _user, const char* _group);
int SuperCloseGroup (SuperUser* _user, const char* _group);
int BanUser(const char* _userName);

void ShutDown();
int ReadMsgLog();
#endif
