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
int Query(struct query *query, CArray<rowItem, rowItem> &Ret, bool flag)
{
	CString cmd;
	cmd = _T("select ");
	if(query->selectedItemCount == 0 || flag == false){
		cmd += _T(" * ");
	}else{
		cmd += query->SI[0].name;
		for(int i = 1; i<query->selectedItemCount; i++){
			cmd += _T(",");
			cmd += query->SI[i].name;
		}
	}

	cmd += _T(" from ");
	cmd += query->from;

	if(query->whereItemCount != 0){
		cmd += _T(" where ");
		if(query->WI[0].valueType == MYSQL_TYPE_STRING||query->WI[0].valueType == MYSQL_TYPE_VAR_STRING||query->WI[0].valueType == MYSQL_TYPE_TIME){
			cmd += query->WI[0].name;
			cmd += _T("=\"");
			cmd += query->WI[0].value;
			cmd += _T("\"");
		}else{
			cmd += query->WI[0].name;
			cmd += _T("=");
			cmd += query->WI[0].value;
		}
		for(int i = 1; i<query->whereItemCount; i++){
			cmd += _T(" and ");
			if(query->WI[0].valueType == MYSQL_TYPE_STRING||query->WI[i].valueType == MYSQL_TYPE_VAR_STRING || query->WI[i].valueType == MYSQL_TYPE_TIME){
				cmd += query->WI[i].name;
				cmd += _T("=\"");
				cmd += query->WI[i].value;
				cmd += _T("\"");
			}else{
				cmd += query->WI[i].name;
				cmd += _T("=");
				cmd += query->WI[i].value;
			}	
		}
	}

	cmd += query->otherTerm;

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
			int num_fields = mysql_num_fields(res); //获取结果集中的列数
			rowItem ri;
			fields = mysql_fetch_fields(res); //获取结果集中的列信息
			ri.fieldCount = num_fields;
			for(int i = 0; i<ri.fieldCount; i++){
				ri.CI[i].name = fields[i].name;//获取结果的列名称
				ri.CI[i].valueType = fields[i].type;//获取结果的列的类型
			}

			if(flag == true && query->selectedItemCount != 0){
				for(int i = 0; i<ri.fieldCount; i++){
					ri.CI[i].chineseName = query->SI[i].chineseName;
				}
			}

			while((record = mysql_fetch_row(res))) {
				rowItem temp;
				temp.fieldCount = ri.fieldCount;
				for(int i = 0; i<num_fields; i++){
					temp.CI[i].chineseName = ri.CI[i].chineseName;
					temp.CI[i].name = ri.CI[i].name;
					temp.CI[i].valueType = ri.CI[i].valueType;
					temp.CI[i].value = record[i];
				}
				Ret.Add(temp);
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
	struct query query;
	query.from = update->from;
	query.selectedItemCount = 0;
	query.whereItemCount = update->whereItemCount;
	for(int i = 0; i<query.whereItemCount; i++){
		query.WI[i].name = update->WI[i].name;
		query.WI[i].valueType = update->WI[i].valueType;
		query.WI[i].value = update->WI[i].value;
	}
	query.otherTerm = _T("");

	int count;
	CArray<rowItem, rowItem> rest;
	Query(&query, rest, false);
	count = rest.GetCount();
	if(count == 0){   //表示没有插入过此类数据，现插入数据
		CString cmd;
		cmd = _T("insert into ");
		cmd += update->from;
		cmd += _T(" set ");
		if(update->SI[0].valueType == MYSQL_TYPE_STRING||update->SI[0].valueType == MYSQL_TYPE_VAR_STRING||update->SI[0].valueType == MYSQL_TYPE_TIME){
			cmd += update->SI[0].name;
			cmd += _T("=\"");
			cmd += update->SI[0].value;
			cmd += _T("\"");
		}
		else{
			cmd += update->SI[0].name;
			cmd += _T("=");
			cmd += update->SI[0].value;
		}

		for(int i = 1; i<update->setItemCount; i++){
			cmd += _T(",");
			if(update->SI[i].valueType == MYSQL_TYPE_STRING||update->SI[i].valueType == MYSQL_TYPE_VAR_STRING || update->SI[i].valueType == MYSQL_TYPE_TIME){
				cmd += update->SI[i].name;
				cmd += _T("=\"");
				cmd += update->SI[i].value;
				cmd += _T("\"");
			}
			else{
				cmd += update->SI[i].name;
				cmd += _T("=");
				cmd += update->SI[i].value;
			}
		}				

		char temp_data[1024]={'\0'};
		WideCharToMultiByte(CP_ACP,0,(LPCWSTR)cmd,-1,(LPSTR)temp_data,sizeof(temp_data),NULL,NULL);
		ret = mysql_query(&mysql, temp_data);
		if(ret)
			ret = mysql_errno(&mysql);
	}else{
		CString cmd;
		cmd = _T("update ");
		cmd += update->from;
		cmd += _T(" set ");
		if(update->SI[0].valueType == MYSQL_TYPE_STRING||update->SI[0].valueType == MYSQL_TYPE_VAR_STRING || update->SI[0].valueType == MYSQL_TYPE_TIME){
			cmd += update->SI[0].name;
			cmd += _T("=\"");
			cmd += update->SI[0].value;
			cmd += _T("\"");
		}
		else{
			cmd += update->SI[0].name;
			cmd += _T("=");
			cmd += update->SI[0].value;
		}

		for(int i = 1; i<update->setItemCount; i++){
			cmd += _T(",");
			if(update->SI[i].valueType == MYSQL_TYPE_STRING||update->SI[i].valueType == MYSQL_TYPE_VAR_STRING || update->SI[i].valueType == MYSQL_TYPE_TIME){
				cmd += update->SI[i].name;
				cmd += _T("=\"");
				cmd += update->SI[i].value;
				cmd += _T("\"");
			}
			else{
				cmd += update->SI[i].name;
				cmd += _T("=");
				cmd += update->SI[i].value;
			}
		}				
		
		if(update->whereItemCount != 0){
			cmd += _T(" where ");
			if(update->WI[0].valueType == MYSQL_TYPE_STRING||update->WI[0].valueType==MYSQL_TYPE_VAR_STRING || update->WI[0].valueType == MYSQL_TYPE_TIME){
				cmd += update->WI[0].name;
				cmd += _T("=\"");
				cmd += update->WI[0].value;
				cmd += _T("\"");
			}
			else{
				cmd += update->WI[0].name;
				cmd += _T("=");
				cmd += update->WI[0].value;
			}

			for(int i = 1; i<update->whereItemCount; i++){
				cmd += _T(",");
				if(update->WI[i].valueType == MYSQL_TYPE_STRING||update->WI[i].valueType==MYSQL_TYPE_VAR_STRING || update->WI[i].valueType == MYSQL_TYPE_TIME){
					cmd += update->WI[i].name;
					cmd += _T("=\"");
					cmd += update->WI[i].value;
					cmd += _T("\"");
				}
				else{
					cmd += update->WI[i].name;
					cmd += _T("=");
					cmd += update->WI[i].value;
				}
			}
		}
		char temp_data[1024]={'\0'};
		WideCharToMultiByte(CP_ACP,0,(LPCWSTR)cmd,-1,(LPSTR)temp_data,sizeof(temp_data),NULL,NULL);
		ret = mysql_query(&mysql, temp_data);
		if(ret)
			ret = mysql_errno(&mysql);
	}
	return ret;
}

int Insert(struct update *update)
{
	int ret = -1;
	struct query query;
	query.from = update->from;
	query.selectedItemCount = 0;
	query.whereItemCount = update->whereItemCount;
	for(int i = 0; i<query.whereItemCount; i++){
		query.WI[i].name = update->WI[i].name;
		query.WI[i].valueType = update->WI[i].valueType;
		query.WI[i].value = update->WI[i].value;
	}
	query.otherTerm = _T("");

	int count;
	CArray<rowItem, rowItem> rest;
	Query(&query, rest, false);
	count = rest.GetCount();
	if(count == 0){   //表示没有插入过此类数据，现插入数据
		CString cmd;
		cmd = _T("insert into ");
		cmd += update->from;
		cmd += _T(" set ");
		if(update->SI[0].valueType == MYSQL_TYPE_STRING||update->SI[0].valueType == MYSQL_TYPE_VAR_STRING||update->SI[0].valueType == MYSQL_TYPE_TIME){
			cmd += update->SI[0].name;
			cmd += _T("=\"");
			cmd += update->SI[0].value;
			cmd += _T("\"");
		}
		else{
			cmd += update->SI[0].name;
			cmd += _T("=");
			cmd += update->SI[0].value;
		}

		for(int i = 1; i<update->setItemCount; i++){
			cmd += _T(",");
			if(update->SI[i].valueType == MYSQL_TYPE_STRING||update->SI[i].valueType == MYSQL_TYPE_VAR_STRING||update->SI[i].valueType == MYSQL_TYPE_TIME){
				cmd += update->SI[i].name;
				cmd += _T("=\"");
				cmd += update->SI[i].value;
				cmd += _T("\"");
			}
			else{
				cmd += update->SI[i].name;
				cmd += _T("=");
				cmd += update->SI[i].value;
			}
		}				

		char temp_data[1024]={'\0'};
		WideCharToMultiByte(CP_ACP,0,(LPCWSTR)cmd,-1,(LPSTR)temp_data,sizeof(temp_data),NULL,NULL);
		ret = mysql_query(&mysql, temp_data);
		if(ret)
			ret = mysql_errno(&mysql);
	}

	return ret;
}