#ifndef _DATA_BASE_H_
#define _DATA_BASE_H_
#include "intern.h"


DB* CreateDataBase (void); 
void DestroyDataBase(DB* _dataBase);
//status GetData(DB* _DB);
Status InsertData(DB* _DB, int _context, MngMsg* _mngmsg);
Status GetData(DB* _DB, int  _context, void* _key, void** _value);


Status UpdateData (DB* _DB, MngMsg* _mngmsg ) ;
#endif
