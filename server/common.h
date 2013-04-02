#if !defined(COMMON_H__)
#define COMMON_H__

#define UNKNOW_MSG -9999;

struct item{
	CString name;
	enum enum_field_types valueType;
	CString value;
};

struct table{
	CString name;
	int itemCount;
	struct item itemValue[0];
};

struct query{
	CString name;
	int itemCount;
	struct item itemValue[0];
};

struct update{
	CString name;
	int itemQuery;
	int itemUpdate;
	struct item itemValue[0];
};

struct results{
	int itemCount;
	struct item itemValue[0];
};

int OpenDB();
void CloseDB();
int Query(struct query &query, struct results **pRet, int *count);
int InsertUniform(struct update *update);
int Insert(struct update *update);

#endif