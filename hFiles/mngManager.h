#ifndef _COMMAND_MENAGER_H_
#define _COMMAND_MENAGER_H_
#include "intern.h"


void CreateMngManager ();  
void DestroyCommandManager(MngManager* _mngManager);

Status ExecuteUserSingIn ( MngMsg* _mngmsg);

Status ExecuteCheckIfUserExist( MngMsg* _mngmsg);

Status ExecuteUserLogIn (MngMsg* _mngmsg);

char* ExecuteFindGroupInfo (MngMsg* _mngmsg);

Status ExecuteUserLogOut (MngMsg* _mngmsg);
Status ExecuteJoinGroup (MngMsg* _mngmsg);
Status ExecuteLeaveGroup (MngMsg* _mngmsg);
Status ExecuteShowAllMyGroups (MngMsg* _mngmsg, char** _groups);
Status ExecuteCloseGroup (MngMsg* _mngmsg);
Status ExecuteSuperLogIn (MngMsg* _mngmsg);
Status ExecuteSuperLogOut (MngMsg* _mngmsg);
Status ExecuteShowAllGroups (MngMsg* _mngmsg);
Status ExecuteCreateNewGroup (MngMsg* _mngmsg);
Status ExecuteBanUser(MngMsg* _mngmsg);
void ShutDown();
Status ExecuteReadMsgLog();

Status ReceiveStatusMngMSG (char* _payload);

#endif
