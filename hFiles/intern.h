#ifndef _INTERN_H_
#define _INTERN_H_
#define PORT 1544
#define IP "127.0.0.1"
#define USER_NAME_SIZE 128
#define PASSWORD_SIZE 128
#define DATA_SIZE 1024
#define HASH_CAPACITY 1000
#define MAX_IN_GROUP 15
#define MAX_GROUPS_NUM 15
#define CHAT_PORT "4444"
typedef enum Commands {JOIN_GROUP=1, LEAVE_GROUP, START_CHAT, SHOW_ALL_MY_GROUPS, CREATE_NEW_GROUP, CLOSE_GROUP, SHOW_ALL_GROUPS,  READ_MSG_LOG, LOG_OUT}Commands;

 enum  {EXIST, NOT_EXIST};

/*NEW*/
typedef enum DataBase {USER_DB, GROUP_DB, EVENT_DB}DataBase;
typedef enum isAdmin {NOT_ADMIN, ADMIN} isAdmin;
typedef enum isBanned {NOT_BANNED, BANNED}isBanned;
typedef enum isOnline {OFFLINE, ONLINE}isOnline;
typedef struct userData userData;
typedef struct groupData groupData;

struct userData
{
	char* m_userName;
	char* m_password;
	int   m_isAdmin;
	char** m_groups;
	int m_isBanned;
	int m_isOnline;
	int m_socketId;
	int m_numOfGroups;
	
};

struct groupData
{
	char* m_groupName;
	char* m_admin;
	int m_numOfUsers;
	char** m_usersInGroup;
	char* m_ipAdress;
};

/************************************************************/



typedef enum MngMsgOpcode {MNG_MSG_SIGN_UP = 1, MNG_MSG_CHECK_IF_USER_EXIST, MNG_MSG_LOG_IN ,MNG_MSG_CREATE_GROUP, MNG_MSG_JOIN_GROUP, MNG_MSG_START_CHAT, MNG_MSG_SHOW_ALL_MY_GROUPS, MNG_MSG_WELCOME} MngMsgOpcode;

typedef enum  Status{OK, ERR} Status ;
typedef struct Init Init;
typedef struct MngManager MngManager;
typedef struct MmgProtocol MngProtocol;
typedef struct SuperUser SuperUser;
typedef struct ChatMsg ChatMsg;
typedef struct CommandManager CommandManager;
typedef struct ChatManager ChatManager;
typedef struct Initializer Initializer;
typedef struct Configs Configs;


typedef struct ChatServer ChatServer;
typedef struct DB DB;
typedef struct userDB userDB;
typedef struct groupDB groupDB;
typedef struct EventDB EventDB;


typedef struct MngMsg {
	int m_socketId;
	int m_opcode;
	char* m_userName ;
	char* m_password  ;
	char* m_data ;
}MngMsg;

#endif
