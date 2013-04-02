#if !defined(COMMON_H__)
#define COMMON_H__

#define UNKNOW_MSG -9999;

struct item{
	CString name;
	enum enum_field_types valueType;
	CString value;
};

struct fieldAttr{
	bool primaryKey;
	enum enum_field_types fieldType;
	CString fieldName;
	CString chineseName;
};

struct table{
	CString name;  //���ݿ����
	CString chineseName;
	int itemKeyCount; //��������
	int itemOthersCount; //�����и���
	struct fieldAttr fieldValue[15]; //���е�����
};

struct query{
	CString name;
	int itemCount;
	struct item itemValue[2];
};

struct update{
	CString name;
	int itemQuery;
	int itemUpdate;
	struct item itemValue[20];
};

struct results{
	int itemCount;
	struct item itemValue[20];
};

int OpenDB();
void CloseDB();
int Query(struct query *query, struct results **pRet, int *count);
int InsertUniform(struct update *update);
int Insert(struct update *update);

#endif