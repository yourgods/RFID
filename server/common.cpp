#include "stdafx.h"
#include "common.h"
#include <mysql.h>
#include <my_global.h>

#pragma comment(lib,"libmysql.lib")

#if 0
enum enum_field_types {INT, DOUBLE, FLOAT, CHAR};

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

#endif

MYSQL *mysql = NULL;

int OpenDB(){
	mysql = mysql_init(NULL);
	if(mysql == NULL){
		return -1; //error, can not init mysql object
	}
	if (mysql_real_connect(mysql, "localhost", "root", "111", NULL, 0, NULL, 0) == NULL) {
		return mysql_errno(mysql);  //error, can not connect the database host.
	}
	return 0;
}

void CloseDB(){
	mysql_close(mysql);
}


//return value: 0 means success, others means error value
int Query(struct query &query, struct results **pRet, int *count)
{
	
	CString cmd;
	*count = 0;
	*pRet = NULL;
	//construct query sentence.
	cmd = _T("select * from ");
	cmd += query.name;
	cmd += _T(" where ");
	if(query.itemValue[0].valueType == MYSQL_TYPE_STRING){
		cmd += query.itemValue[0].name;
		cmd += _T("=\"");
		cmd += query.itemValue[0].value;
		cmd += _T("\"");
	}else{
		cmd += query.itemValue[0].name;
		cmd += _T("=");
		cmd += query.itemValue[0].value;
	}

	for(int i = 1; i<query.itemCount; i++){
		cmd += _T(",");
		if(query.itemValue[i].valueType == MYSQL_TYPE_STRING){
			cmd += query.itemValue[i].name;
			cmd += _T("=\"");
			cmd += query.itemValue[i].value;
			cmd += _T("\"");
		}
		else{
			cmd += query.itemValue[i].name;
			cmd += _T("=");
			cmd += query.itemValue[i].value;
		}
	}

	char temp_data[1024]={'\0'};
	WideCharToMultiByte(CP_ACP,0,(LPCWSTR)cmd,-1,(LPSTR)temp_data,sizeof(temp_data),NULL,NULL);
	int ret = UNKNOW_MSG;
	ret = mysql_query(mysql, temp_data);
	if(!ret){
		MYSQL_RES *res = 0;
		MYSQL_FIELD  *fields = 0;
		MYSQL_ROW record;
		res = mysql_store_result(mysql);
		if(res){//there are rows
			*count = (int)mysql_num_rows(res); //获取结果集中的行数
			if(pRet != NULL){
				int num_fields = mysql_num_fields(res); //获取结果集中的列数
				struct item *temp = (struct item *)malloc(sizeof(struct item)*num_fields);  //用于记录结果集中的列的名称和类型
				memset(temp, 0x00, sizeof(struct item)*num_fields);
				fields = mysql_fetch_fields(res); //获取结果集中的列信息
				for(int i = 0; i<num_fields; i++){
					temp[i].name.Format(_T("%s"), fields[i].name); //获取结果的列名称
					temp[i].valueType = fields[i].type; //获取结果的列的类型
				}

				int size = sizeof(struct results) + *count*sizeof(struct item);
				struct results *r = (struct results *)malloc(size);
				memset(r, 0x00, sizeof(struct results)*(*count));
				r->itemCount = *count;
				int resCount = 0;
				while((record = mysql_fetch_row(res))) {
					for(int i = 0; i<num_fields; i++){
						r->itemValue[resCount].name = temp[i].name;
						r->itemValue[resCount].valueType = temp[i].valueType;
						r->itemValue[resCount].value.Format(_T("%s"), record[i]);
					}
					resCount++;
				}
				free(temp);
				*pRet = r;
			}
			mysql_free_result(res);
			ret = 0;
		}else{ //mysql_store_result() returned nothing, should it have?
			if(mysql_field_count(mysql)==0){ //不应该返回结果集
				ret = 0;
			}else{
				ret = mysql_errno(mysql); //应该返回结果集但未返回，返回错误码
			}
		}
	}
	return ret;
}

int InsertUniform(struct update *update)
{
	int ret = 0;
	struct query *query = (struct query *)malloc(sizeof(struct query) + update->itemQuery*sizeof(struct item));
	query->itemCount = update->itemQuery;
	query->name = update->name;
	for(int i = 0; i<query->itemCount; i++){
		query->itemValue[i] = update->itemValue[i];
	}

	int count;
	Query(*query, NULL, &count);
	free(query);
	if(count == 0){   //表示没有插入过此类数据，现插入数据
		CString cmd;
		cmd = _T("insert into ");
		cmd += update->name;
		cmd += _T(" set ");
		if(update->itemValue[update->itemQuery].valueType == MYSQL_TYPE_STRING){
			cmd += update->itemValue[update->itemQuery].name;
			cmd += _T("=\"");
			cmd += update->itemValue[update->itemQuery].value;
			cmd += _T("\"");
		}
		else{
			cmd += update->itemValue[update->itemQuery].name;
			cmd += _T("=");
			cmd += update->itemValue[update->itemQuery].value;
		}
		for(int i = update->itemQuery + 1; i<update->itemQuery + update->itemUpdate; i++){
			cmd += _T(",");
			if(update->itemValue[i].valueType == MYSQL_TYPE_STRING){
				cmd += update->itemValue[i].name;
				cmd += _T("=\"");
				cmd += update->itemValue[i].value;
				cmd += _T("\"");
			}
			else{
				cmd += update->itemValue[i].name;
				cmd += _T("=");
				cmd += update->itemValue[i].value;
			}
		}

		char temp_data[1024]={'\0'};
		WideCharToMultiByte(CP_ACP,0,(LPCWSTR)cmd,-1,(LPSTR)temp_data,sizeof(temp_data),NULL,NULL);
		int ret = UNKNOW_MSG;
		ret = mysql_query(mysql, temp_data);
	}else{
		CString cmd;
		cmd = _T("update ");
		cmd += update->name;
		cmd += _T(" set ");
		if(update->itemValue[update->itemQuery].valueType == MYSQL_TYPE_STRING){
			cmd += update->itemValue[update->itemQuery].name;
			cmd += _T("=\"");
			cmd += update->itemValue[update->itemQuery].value;
			cmd += _T("\"");
		}
		else{
			cmd += update->itemValue[update->itemQuery].name;
			cmd += _T("=");
			cmd += update->itemValue[update->itemQuery].value;
		}
		for(int i = update->itemQuery + 1; i<update->itemQuery + update->itemUpdate; i++){
			cmd += _T(",");
			if(update->itemValue[i].valueType == MYSQL_TYPE_STRING){
				cmd += update->itemValue[i].name;
				cmd += _T("=\"");
				cmd += update->itemValue[i].value;
				cmd += _T("\"");
			}
			else{
				cmd += update->itemValue[i].name;
				cmd += _T("=");
				cmd += update->itemValue[i].value;
			}
		}
		cmd += _T(" where ");
		if(update->itemValue[0].valueType == MYSQL_TYPE_STRING){
			cmd += update->itemValue[0].name;
			cmd += _T("=\"");
			cmd += update->itemValue[0].value;
			cmd += _T("\"");
		}
		else{
			cmd += update->itemValue[0].name;
			cmd += _T("=");
			cmd += update->itemValue[0].value;
		}

		for(int i = 1; i<update->itemQuery; i++){
			cmd += _T(",");
			if(update->itemValue[i].valueType == MYSQL_TYPE_STRING){
				cmd += update->itemValue[i].name;
				cmd += _T("=\"");
				cmd += update->itemValue[i].value;
				cmd += _T("\"");
			}
			else{
				cmd += update->itemValue[i].name;
				cmd += _T("=");
				cmd += update->itemValue[i].value;
			}
		}
		char temp_data[1024]={'\0'};
		WideCharToMultiByte(CP_ACP,0,(LPCWSTR)cmd,-1,(LPSTR)temp_data,sizeof(temp_data),NULL,NULL);
		int ret = UNKNOW_MSG;
		ret = mysql_query(mysql, temp_data);
	}
	return ret;
}

int Insert(struct update *update)
{
	int ret = 0;
	struct query *query = (struct query *)malloc(sizeof(struct query) + update->itemQuery*sizeof(struct item));
	query->itemCount = update->itemQuery;
	query->name = update->name;
	for(int i = 0; i<query->itemCount; i++){
		query->itemValue[i] = update->itemValue[i];
	}

	int count;
	Query(*query, NULL, &count);
	free(query);
	if(count == 0){   //表示没有插入过此类数据，现插入数据
		CString cmd;
		cmd = _T("insert into ");
		cmd += update->name;
		cmd += _T(" set ");
		if(update->itemValue[update->itemQuery].valueType == MYSQL_TYPE_STRING){
			cmd += update->itemValue[update->itemQuery].name;
			cmd += _T("=\"");
			cmd += update->itemValue[update->itemQuery].value;
			cmd += _T("\"");
		}
		else{
			cmd += update->itemValue[update->itemQuery].name;
			cmd += _T("=");
			cmd += update->itemValue[update->itemQuery].value;
		}
		for(int i = update->itemQuery + 1; i<update->itemQuery + update->itemUpdate; i++){
			cmd += _T(",");
			if(update->itemValue[i].valueType == MYSQL_TYPE_STRING){
				cmd += update->itemValue[i].name;
				cmd += _T("=\"");
				cmd += update->itemValue[i].value;
				cmd += _T("\"");
			}
			else{
				cmd += update->itemValue[i].name;
				cmd += _T("=");
				cmd += update->itemValue[i].value;
			}
		}

		char temp_data[1024]={'\0'};
		WideCharToMultiByte(CP_ACP,0,(LPCWSTR)cmd,-1,(LPSTR)temp_data,sizeof(temp_data),NULL,NULL);
		int ret = UNKNOW_MSG;
		ret = mysql_query(mysql, temp_data);
	}

	return ret;
}