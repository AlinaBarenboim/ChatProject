#ifndef _GROUP_DATA_BASE_H_
#define _GROUP_DATA_BASE_H_
#include "intern.h"


groupDB* CreateGroupDataBase (size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc);
void DestroyGroupDataBase(groupDB* _dataBase);
Status InsertDataToGroupDataBase (groupDB* _groupDb, void* _key, void* _value);
Status GetDataFromGroupDataBase (groupDB* _groupDb, void* _key, void** _value);
Status RemoveDataFromGroupDataBase (groupDB* _groupDb, void* _key);
Status UpdateGroupInGroupDataBase (groupDB* _groupDb ,MngMsg* _mngmsg);

groupData* CreateGroupDataPack (MngMsg* _mngmsg, groupDB* _groupDb);
#endif
