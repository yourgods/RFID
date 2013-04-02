#include "stdafx.h"
#include "common.h"
#include <mysql.h>
#include <mysql_com.h>
#include <my_global.h>

#pragma comment(lib,"libmysql.lib")

MYSQL mysql;

int OpenDB(){
	if(mysql_init(&mysql) == NULL){
		return UNKNOW_MSG;
	}
	if (mysql_real_connect(&mysql, "localhost", "root", "1111", "mydb", 3306, NULL,0) == NULL) {
		return mysql_errno(&mysql);  //error, can not connect the database host.
	}
	return 0;
}

void CloseDB(){
	mysql_close(&mysql);
}


//return value: 0 means success, others means error value
int Query(struct query *query, struct results **pRet, int *count)
{
	
	CString cmd;
	*count = 0;
	*pRet = NULL;
	//construct query sentence.
	cmd = _T("select * from ");
	cmd += query->name;
	if(query->itemCount != 0){
		cmd += _T(" where ");
		if(query->itemValue[0].valueType == MYSQL_TYPE_STRING||query->itemValue[0].valueType == MYSQL_TYPE_VAR_STRING){
			cmd += query->itemValue[0].name;
			cmd += _T("=\"");
			cmd += query->itemValue[0].value;
			cmd += _T("\"");
		}else{
			cmd += query->itemValue[0].name;
			cmd += _T("=");
			cmd += query->itemValue[0].value;
		}

		for(int i = 1; i<query->itemCount; i++){
			cmd += _T(",");
			if(query->itemValue[i].valueType == MYSQL_TYPE_STRING||query->itemValue[0].valueType == MYSQL_TYPE_VAR_STRING){
				cmd += query->itemValue[i].name;
				cmd += _T("=\"");
				cmd += query->itemValue[i].value;
				cmd += _T("\"");
			}
			else{
				cmd += query->itemValue[i].name;
				cmd += _T("=");
				cmd += query->itemValue[i].value;
			}
		}
	}

	char temp_data[1024]={'\0'};
	WideCharToMultiByte(CP_ACP,0,(LPCWSTR)cmd,-1,(LPSTR)temp_data,sizeof(temp_data),NULL,NULL);
	int ret = UNKNOW_MSG;
	ret = mysql_query(&mysql, temp_data);
	if(!ret){
		MYSQL_RES *res = 0;
		MYSQL_FIELD  *fields = 0;
		MYSQL_ROW record;
		res = mysql_store_result(&mysql);
		if(res){//there are rows
			*count = (int)mysql_num_rows(res); //获取结果集中的行数
			if(pRet != NULL){
				int num_fields = mysql_num_fields(res); //获取结果集中的列数
				struct results rets;
				memset(&rets, 0x00, sizeof(rets));
				//struct item *temp = (struct item *)malloc(sizeof(struct item)*num_fields);  //用于记录结果集中的列的名称和类型
				//memset(temp, 0x00, sizeof(struct item)*num_fields);
				fields = mysql_fetch_fields(res); //获取结果集中的列信息
				rets.itemCount = num_fields;
				for(int i = 0; i<rets.itemCount; i++){
					rets.itemValue[i].name.Format(_T("%s"), fields[i].name); //获取结果的列名称
					rets.itemValue[i].valueType = fields[i].type; //获取结果的列的类型
				}

				int size = sizeof(struct results)*(*count);
				struct results *r = (struct results *)malloc(size);
				memset(r, 0x00, sizeof(struct results)*(*count));
				int resCount = 0;
				while((record = mysql_fetch_row(res))) {
					r[resCount].itemCount = rets.itemCount;
					for(int i = 0; i<num_fields; i++){
						r[resCount].itemValue[i].name = rets.itemValue[i].name;
						r[resCount].itemValue[i].valueType = rets.itemValue[i].valueType;
						r[resCount].itemValue[i].value.Format(_T("%s"), record[i]);
					}
					resCount++;
				}
				*pRet = r;
			}
			mysql_free_result(res);
			ret = 0;
		}else{ //mysql_store_result() returned nothing, should it have?
			if(mysql_field_count(&mysql)==0){ //不应该返回结果集
				ret = 0;
			}else{
				ret = mysql_errno(&mysql); //应该返回结果集但未返回，返回错误码
			}
		}
	}
	return ret;
}

int InsertUniform(struct update *update)
{
	int ret = 0;
	struct query *query = (struct query *)malloc(sizeof(struct query));
	query->itemCount = update->itemQuery;
	query->name = update->name;
	for(int i = 0; i<query->itemCount; i++){
		query->itemValue[i].name = update->itemValue[i].name;
		query->itemValue[i].valueType = update->itemValue[i].valueType;
		query->itemValue[i].value = update->itemValue[i].value;
	}

	int count;
	Query(query, NULL, &count);
	free(query);
	if(count == 0){   //表示没有插入过此类数据，现插入数据
		CString cmd;
		cmd = _T("insert into ");
		cmd += update->name;
		cmd += _T(" set ");
		if(update->itemValue[update->itemQuery].valueType == MYSQL_TYPE_STRING||update->itemValue[update->itemQuery].valueType == MYSQL_TYPE_VAR_STRING){
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
			if(update->itemValue[i].valueType == MYSQL_TYPE_STRING||update->itemValue[i].valueType == MYSQL_TYPE_VAR_STRING){
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
		ret = mysql_query(&mysql, temp_data);
	}else{
		CString cmd;
		cmd = _T("update ");
		cmd += update->name;
		cmd += _T(" set ");
		if(update->itemValue[update->itemQuery].valueType == MYSQL_TYPE_STRING||update->itemValue[update->itemQuery].valueType==MYSQL_TYPE_VAR_STRING){
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
			if(update->itemValue[i].valueType == MYSQL_TYPE_STRING||update->itemValue[i].valueType==MYSQL_TYPE_VAR_STRING){
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
		if(update->itemValue[0].valueType == MYSQL_TYPE_STRING||update->itemValue[0].valueType==MYSQL_TYPE_VAR_STRING){
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
			if(update->itemValue[i].valueType == MYSQL_TYPE_STRING||update->itemValue[i].valueType==MYSQL_TYPE_VAR_STRING){
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
		ret = mysql_query(&mysql, temp_data);
	}
	return ret;
}

int Insert(struct update *update)
{
	int ret = 0;
	struct query *query = (struct query *)malloc(sizeof(struct query));
	query->itemCount = update->itemQuery;
	query->name = update->name;
	for(int i = 0; i<query->itemCount; i++){
		query->itemValue[i].name = update->itemValue[i].name;
		query->itemValue[i].valueType = update->itemValue[i].valueType;
		query->itemValue[i].value = update->itemValue[i].value;
	}

	int count;
	Query(query, NULL, &count);
	free(query);
	if(count == 0){   //表示没有插入过此类数据，现插入数据
		CString cmd;
		cmd = _T("insert into ");
		cmd += update->name;
		cmd += _T(" set ");
		if(update->itemValue[update->itemQuery].valueType == MYSQL_TYPE_STRING||update->itemValue[update->itemQuery].valueType == MYSQL_TYPE_VAR_STRING){
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
			if(update->itemValue[i].valueType == MYSQL_TYPE_STRING||update->itemValue[i].valueType == MYSQL_TYPE_VAR_STRING){
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
		ret = mysql_query(&mysql, temp_data);
	}

	return ret;
}