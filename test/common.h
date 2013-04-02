#if !defined(COMMON_H__)
#define COMMON_H__

#include "testDoc.h"
#define UNKNOW_MSG -9999;


//Used for query function
typedef struct _SELECTEDITEM{
	CString name;
	CString chineseName;
} selectedItem;

typedef struct _WHEREITEM{
	CString name;
	enum enum_field_types valueType;
	CString value;
} whereItem;

struct query{
	CString from;
	int selectedItemCount;
	int whereItemCount;
	CString otherTerm;
	selectedItem SI[20];
	whereItem WI[20];
};

//Used for query function results
typedef struct _COLITEM{
	CString name;
	CString chineseName;
	enum enum_field_types valueType;
	CString value;
} colItem;

typedef struct _ROWITEM{
	int fieldCount;
	colItem CI[20];
} rowItem;

//used for update function
typedef struct _SETITEM{
	CString name;
	enum enum_field_types valueType;
	CString value;
} setItem;

 struct update{
	CString from;
	int setItemCount;
	int whereItemCount;
	setItem SI[20];
	whereItem WI[20];
};

//Used for database table define
struct fieldAttr{
	bool primaryKey;
	enum enum_field_types fieldType;
	CString fieldName;
	CString chineseName;
};

struct table{
	CString name;  //数据库表名
	CString chineseName;
	int (*lpfCreateDlg)(CtestDoc *doc);
	int (*lpfQueryRight)(CArray<rowItem, rowItem> &rest);
	int itemKeyCount; //主键个数
	int itemOthersCount; //其他列个数
	struct fieldAttr fieldValue[15]; //各列的属性
};


//
//
//struct item{
//	CString name;
//	enum enum_field_types valueType;
//	CString value;
//};
//
//typedef struct results{
//	int itemCount;
//	struct item itemValue[20];
//} db_rest;
//
//struct query{
//	CString name;
//	int itemColCount;
//	int itemCount;
//	struct item ColItemValue[20];
//	struct item itemValue[20];
//};
//
//struct update{
//	CString name;
//	int itemQuery;
//	int itemUpdate;
//	struct item itemValue[20];
//};
int OpenDB();
void CloseDB();
int Query(struct query *query, CArray<rowItem, rowItem> &ret, bool flag = true);
int InsertUniform(struct update *update);
int Insert(struct update *update);

#endif