#ifndef _CHAT_PROTOCOL_H_
#define _CHAT_PROTOCOL_H_
#include "intern.h"




ChatProtocol* CreateChatProtocol (const char* _configFile);  
void DestroyChatProtocol(ChatProtocol* _chatProtocol);
Status SendChatMsg (ChatMsg* _chatmsg);
ChatMsg* ReceiveChatMsg (char* _payload);

#endif
