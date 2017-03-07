#ifndef _MNG_PROTOCOL_H_
#define _MNG_PROTOCOL_H_
#include "intern.h"

MngMsg* ReceiveMngMsg (char* _payload);
MngMsg* StringToStract (char* _payload) ;
char* StractToString (MngMsg* _mngmsg);

MngProtocol* CreateMngProtocol (const char* _configFile); /*from initializer*/ 
void DestroyCommandProtocol(MngProtocol* _commandProtocol);
MngMsg* SendMngMsg (MngMsg* _mngmsg, int* _sockId); /*TODO return?*/

#endif
