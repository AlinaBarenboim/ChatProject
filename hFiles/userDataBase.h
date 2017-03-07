#ifndef _USER_DATA_BASE_H_
#define _USER_DATA_BASE_H_
#include "intern.h"


userDB* CreateUserDataBase (size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFun); 
void DestroyUserDataBase(userDB* _dataBase);
Status InsertDataToUserDataBase (userDB* _userDb, void* _key, void* _value);
Status GetDataFromUserDataBase (userDB* _userDb, void* _key, void** _value);
Status RemoveDataFromUserDataBase (userDB* _userDb, void* _key);

userData* CreateUserDataPack (MngMsg* _mngmsg);

Status UpdateDataInUserDataBase (userDB* _userDb ,MngMsg* _mngmsg);
#endif
