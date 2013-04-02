#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h> 
#include <iostream>
#include "message.h"
#include "database.h"
#include "common.h"

bool parseBusDepart(char *msg, struct update *update)
{
	CString temp(msg);
	update->from = _T("bus_depart");
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].name.Compare(update->from) == 0)
			break;
	}

	update->whereItemCount = Table[index].itemKeyCount;
	for(int i = 0; i<update->whereItemCount; i++){
		update->WI[i].name = Table[index].fieldValue[i].fieldName;
		update->WI[i].valueType = Table[index].fieldValue[i].fieldType;
	}

	update->setItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
	for(int i = 0;i<Table[index].itemKeyCount; i++){
		int t = temp.Find(_T(","));
		update->SI[i].name = Table[index].fieldValue[i].fieldName;
		update->SI[i].valueType = Table[index].fieldValue[i].fieldType;
		update->SI[i].value = temp.Left(t).Trim();
		update->WI[i].value = update->SI[i].value;
		temp.Delete(0, t+1);
	}

	for(int i = Table[index].itemKeyCount; i<update->setItemCount; i++){
		int t = temp.Find(_T(","));
		update->SI[i].name = Table[index].fieldValue[i].fieldName;
		update->SI[i].valueType = Table[index].fieldValue[i].fieldType;
		update->SI[i].value = temp.Left(t).Trim();
		temp.Delete(0, t+1);
	}

	return true;
	//remark is null
}

bool parseBusRun(char *msg, struct update *update)
{
	CString temp(msg);
	update->from = _T("bus_run");
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].name.Compare(update->from) == 0)
			break;
	}

	update->whereItemCount = Table[index].itemKeyCount;
	for(int i = 0; i<update->whereItemCount; i++){
		update->WI[i].name = Table[index].fieldValue[i].fieldName;
		update->WI[i].valueType = Table[index].fieldValue[i].fieldType;
		update->WI[i].value = _T("");
	}

	update->setItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount - 1;
	for(int i = 0;i<update->setItemCount; i++){
		int t = temp.Find(_T(","));
		update->SI[i].name = Table[index].fieldValue[i+1].fieldName;
		update->SI[i].valueType = Table[index].fieldValue[i+1].fieldType;
		update->SI[i].value = temp.Left(t).Trim();
		temp.Delete(0, t+1);
	}

	return true;
	//remark is null
}

bool parseBusStudent(char *msg, struct update *update)
{
	CString temp(msg);
	update->from = _T("bus_stu");
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].name.Compare(update->from) == 0)
			break;
	}

	update->whereItemCount = Table[index].itemKeyCount;
	for(int i = 0; i<update->whereItemCount; i++){
		update->WI[i].name = Table[index].fieldValue[i].fieldName;
		update->WI[i].valueType = Table[index].fieldValue[i].fieldType;
	}

	update->setItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
	for(int i = 0;i<Table[index].itemKeyCount; i++){
		int t = temp.Find(_T(","));
		update->SI[i].name = Table[index].fieldValue[i].fieldName;
		update->SI[i].valueType = Table[index].fieldValue[i].fieldType;
		update->SI[i].value = temp.Left(t).Trim();
		update->WI[i].value = update->SI[i].value;
		temp.Delete(0, t+1);
	}

	for(int i = Table[index].itemKeyCount; i<update->setItemCount; i++){
		int t = temp.Find(_T(","));
		update->SI[i].name = Table[index].fieldValue[i].fieldName;
		update->SI[i].valueType = Table[index].fieldValue[i].fieldType;
		update->SI[i].value = temp.Left(t).Trim();
		temp.Delete(0, t+1);
	}

	return true;
	//remark is null
}

// parseMsg return value as follows:
//
//·         CR_COMMANDS_OUT_OF_SYNC 
//
//以不恰当的顺序执行了命令。 
//
//·         CR_SERVER_GONE_ERROR 
//
//MySQL服务器不可用。 
//
//·         CR_SERVER_LOST 
//
//在查询过程中，与服务器的连接丢失。 
//
//·         CR_UNKNOWN_ERROR 
//
//出现未知错误。 

#if 0
int parseMsg(MYSQL *mysql, char *msg, CString &str){
	int ret = UNKNOW_MSG;
	MYSQL_RES *res = NULL;
	char *prevPos = 0, *currPos = 0;
	char inst[256] = {0};
	currPos = strstr(msg, ",");
	int msgId = char2int(msg, currPos-prevPos);
	msg = currPos + 1;
	switch(msgId){
	case 1: //bus_depart message
		struct bus_depart Bus_Depart;
		parseBusDepart(msg, &Bus_Depart);
		
		break;
	case 2: //bus_run message
		struct bus_run Bus_Run;
		parseBusRun(msg, &Bus_Run);
		
		break;
	case 3: //bus_stu message
		struct bus_stu Bus_Student;
		parseBusStudent(msg, &Bus_Student);
		
		break;
	default:
		
		ret = 0;
		break;
	}
	return ret;
}
#endif