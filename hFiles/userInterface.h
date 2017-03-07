#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

typedef enum OpcodesLevelOne {SIGN_UP = 1, LOG_IN, LOGOUT}OpcodesLevelOne;


/*typedef enum OpcodesLevelTwoUser {JOIN_GROUP = 1, LEAVE_GROUP, SEND_MSG, SHOW_ALL_MY_GROUPS, CREATE_NEW_GROUP, CLOSE_GROUP, SHOW_ALL_GROUPS, LOG_OUT}OpcodesLevelTwoUser;*/

/*typedef enum OpcodesLevelTwoSuperUser {SEND_MSG = 1,  SEND_MSG_TO_ALL_GROUPS, SHUT_DOWN, SHOW_ALL_GROUPS,  CREATE_NEW_GROUP, CLOSE_GROUP,BANN_USER, SEND_MSG_TO_ALL_GROUPS, READ_MSG_LOG , LOG_OUT}OpcodesLevelTwoSuperUser;*/

typedef struct UI UI;

/*UI* CreateUI ();*/
/*int RunUI ();*/
/*void UIDestroy();*/


int LogIn ( const char* _userName, const char* _password);
int LogOut (MngMsg* _user );
int SignUp (const char* _userName, const char* _password);
int SignOut (const char* _userName, const char* _password);

#endif
