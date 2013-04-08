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

	update->setItemCount = Table[index].itemOthersCount;
	for(int i = 0;i<update->setItemCount; i++){
		int t = temp.Find(_T(","));
		update->SI[i].name = Table[index].fieldValue[i+Table[index].itemKeyCount].fieldName;
		update->SI[i].valueType = Table[index].fieldValue[i+Table[index].itemKeyCount].fieldType;
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

int BusRunInfoQueryRight(CArray<rowItem, rowItem> &rest)
{
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].name.Compare(_T("bus_run")) == 0)
			break;
	}

	struct query query;
	query.from = _T("bus_run");
	query.selectedItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
	for(int i = 0; i<query.selectedItemCount; i++){
		query.SI[i].name = Table[index].fieldValue[i].fieldName;
		query.SI[i].chineseName = Table[index].fieldValue[i].chineseName;
	}
	query.whereItemCount = 0;
	query.otherTerm = _T("");
	Query(&query, rest);
	if(rest.GetCount() == 0){
		rowItem temp;
		temp.fieldCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
		for(int i = 0; i<temp.fieldCount; i++){
			temp.CI[i].chineseName = Table[index].fieldValue[i].chineseName;
			temp.CI[i].name = Table[index].fieldValue[i].fieldName;
			temp.CI[i].valueType = Table[index].fieldValue[i].fieldType;
		}
		rest.Add(temp);
	}

	return TRUE;
}

int BusDepartInfoQueryRight(CArray<rowItem, rowItem> &rest)
{
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].name.Compare(_T("bus_depart")) == 0)
			break;
	}

	struct query query;
	query.from = _T("bus_depart");
	query.selectedItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
	for(int i = 0; i<query.selectedItemCount; i++){
		query.SI[i].name = Table[index].fieldValue[i].fieldName;
		query.SI[i].chineseName = Table[index].fieldValue[i].chineseName;
	}
	query.whereItemCount = 0;
	query.otherTerm = _T("");
	Query(&query, rest);
	if(rest.GetCount() == 0){
		rowItem temp;
		temp.fieldCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
		for(int i = 0; i<temp.fieldCount; i++){
			temp.CI[i].chineseName = Table[index].fieldValue[i].chineseName;
			temp.CI[i].name = Table[index].fieldValue[i].fieldName;
			temp.CI[i].valueType = Table[index].fieldValue[i].fieldType;
		}
		rest.Add(temp);
	}

	return TRUE;
}

int BusStudentInfoQueryRight(CArray<rowItem, rowItem> &rest)
{
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].name.Compare(_T("bus_stu")) == 0)
			break;
	}

	struct query query;
	query.from = _T("bus_stu");
	query.selectedItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
	for(int i = 0; i<query.selectedItemCount; i++){
		query.SI[i].name = Table[index].fieldValue[i].fieldName;
		query.SI[i].chineseName = Table[index].fieldValue[i].chineseName;
	}
	query.whereItemCount = 0;
	query.otherTerm = _T("");
	Query(&query, rest);
	if(rest.GetCount() == 0){
		rowItem temp;
		temp.fieldCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
		for(int i = 0; i<temp.fieldCount; i++){
			temp.CI[i].chineseName = Table[index].fieldValue[i].chineseName;
			temp.CI[i].name = Table[index].fieldValue[i].fieldName;
			temp.CI[i].valueType = Table[index].fieldValue[i].fieldType;
		}
		rest.Add(temp);
	}

	return TRUE;
}