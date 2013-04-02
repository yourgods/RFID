#if !defined(DATABASE_H__)
#define DATABASE_H__
#include "common.h"

extern struct table Table[9];

struct _DATABASE_ENTRY_
{
	CString name;
	int (*lpfQuery)(CString school_ID, struct results **pRet, int *count);
	int (*lpfInsertUniform)(struct  update *update);
	int (*lpfInsert)();
};

//ѧУ��Ϣ������
int SchoolInfoQuery(CString school_ID, struct results **pRet, int *count);
int SchoolInfoInsertUniform(struct  update *update);
int SchoolInfoInsert();


#endif