#ifndef _CHAT_MENAGER_H_
#define _CHAT_MENAGER_H_
#include "intern.h"


ChatManager* CreateChatManager (ChatProtocol* _chatProtocol);  
void DestroyChatManager(ChatManager* _chatManager);

Status ExecuteSendChatMsgToAllGroups(ChatMsg* _chatmsg);
Status ExecuteSendChatMSG (ChatMsg* _chatmsg);
Status ReceiveStatusChatMSG (char* _payload);

#endif
