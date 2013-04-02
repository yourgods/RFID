#include "stdafx.h"
#include "database.h"
#include "config.h"
#include "sqlite3.h"
#include "tools.h"
#include "message.h"

sqlite3* pDB = NULL;
char send_data[1024];
UINT pe_number=0;

static BOOL TableExist(char *table);

BOOL CheckDataBaseExist()//返回1，则表示数据库已存在；返回0，则表示不存在并创建
{
	convert_char(_T("\\ResidentFlash\\Config\\RFIDData.db"));

	int res = sqlite3_open(send_data, &pDB);
	if(res)
	{
		AfxMessageBox(_T("打开数据库错误"));
		return FALSE;
	}

	char* errMsg;
	CString strTemp;
	char temp_data[1024]={'\0'};
	char *temp1;
	int i;
	//Create bus_depart table
	if(!TableExist("bus_depart")){
		strTemp = "create table bus_depart (bus_run_ID CHAR(30) NOT NULL PRIMARY KEY, bus_ID CHAR(10), driver_ID CHAR(10), teacher_ID CHAR(10), road_ID CHAR(10), student_number INTEGER, remark VARCHAR(255))";
		temp1 = (char *)(char*)(LPCTSTR)strTemp;
		for(i=0;i<strTemp.GetLength();i++)
		{
			temp_data[i] = *temp1++;
			if(*temp1 == 0)
				temp1++;
			if(temp_data[i] == 0)
				break;
		}
		temp_data[i] = 0;	
		res = sqlite3_exec(pDB,temp_data,0,0, &errMsg);
		if (res != SQLITE_OK)
		{
	#ifdef DEBUG_YFH
			AfxMessageBox(_T("创建表1错误"));
	#endif
		}
	}else{
		//AfxMessageBox(_T("表1已经存在!"));
	}

	//Create bus_stu table
	if(!TableExist("bus_stu")){
		strTemp = "create table bus_stu (bus_run_ID CHAR(20) NOT NULL, student_ID CHAR(10) NOT NULL, up_station_ID CHAR(10), up_time TIME, down_station_ID CHAR(10), down_time TIME, remark VARCHAR(255),  PRIMARY KEY(bus_run_ID, student_ID))";
		temp1 = (char *)(char *)(LPCTSTR)strTemp;
		for(i=0;i<strTemp.GetLength();i++)
		{
			temp_data[i] = *temp1++;
			if(*temp1 == 0)
				temp1++;
			if(temp_data[i] == 0)
				break;
		}
		temp_data[i] = 0;	
		res = sqlite3_exec(pDB,temp_data,0,0, &errMsg);
		if (res != SQLITE_OK)
		{
	#ifdef DEBUG_YFH
			AfxMessageBox(_T("创建表2错误"));
	#endif
		}
	}else{
		//AfxMessageBox(_T("表2已经存在!"));
	}

	//create schoolbus table
	if(!TableExist("schoolbus")){
		strTemp = "create table schoolbus (ID INTEGER PRIMARY KEY AUTOINCREMENT, StudentName VARCHAR(16),ParentPhoneNum VARCHAR(16),UpBusTime VARCHAR(10),IsUpMessageSended VARCHAR(10),DownBusTime VARCHAR(10),IsDownMessageSended VARCHAR(10),BusCode VARCHAR(10))";
		temp1 = (char *)(char*)(LPCTSTR)strTemp;
		for(i=0;i<strTemp.GetLength();i++)
		{
			temp_data[i] = *temp1++;
			if(*temp1 == 0)
				temp1++;
			if(temp_data[i] == 0)
				break;
		}
		temp_data[i] = 0;	
		res = sqlite3_exec(pDB,temp_data,0,0, &errMsg);
		if (res != SQLITE_OK)
		{
			AfxMessageBox(_T("创建表3错误"));
		}
	}else{
		//AfxMessageBox(_T("表3已经存在!"));
	}

	//AfxMessageBox(_T("现在开始测试数据库!"));
	TestDatabase();

	return TRUE;
}

BOOL CloseDatabase()
{
	sqlite3_close(pDB);
	pDB = NULL;
	return TRUE;
}

static BOOL TableExist(char *table)
{
	sqlite3_stmt *stmt = NULL;
	char pTempCmd[256];
	char *pErroMsg = NULL;
	sprintf(pTempCmd, "select count(*) from sqlite_master where type='table' and name='%s'", table);
	if(SQLITE_OK != sqlite3_prepare(pDB, pTempCmd, strlen(pTempCmd), &stmt, NULL)){
		return FALSE;
	}

	int no = 0;
	while(sqlite3_step(stmt)  == SQLITE_ROW){
		no = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	if(no == 0)
		return FALSE;
	return TRUE;
}

BOOL InsertBusDepart(struct bus_depart &Bus_Depart)
{
	sqlite3_stmt *stmt = NULL;
	char pTempCmd[256];
	char *pErroMsg = NULL;
	sprintf(pTempCmd, "select * from bus_depart where bus_run_ID = \"%s\"", Bus_Depart.bus_run_ID);
	if(SQLITE_OK != sqlite3_prepare(pDB, pTempCmd, strlen(pTempCmd), &stmt, NULL))
	{
		return FALSE;
	}

	if(SQLITE_ROW != sqlite3_step(stmt)){
		sqlite3_finalize(stmt);
		sprintf(pTempCmd, "insert into bus_depart values (\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", %s, \"%s\")", Bus_Depart.bus_run_ID, Bus_Depart.bus_ID, Bus_Depart.driver_ID, Bus_Depart.teacher_ID, Bus_Depart.road_ID, Bus_Depart.student_number, "");
		if( SQLITE_OK!=sqlite3_exec(pDB,pTempCmd,0,0,&pErroMsg))
		{
	#ifdef DEBUG_YFH
			CString error(pErroMsg);
			CString tmp;
			tmp = _T("插入bus_depart错误，错误原因:") + error;
			AfxMessageBox(tmp);
	#endif
			return FALSE; 
		}
	}
	return TRUE;
}

BOOL InsertBusStu(struct bus_stu &Bus_Student)
{
	sqlite3_stmt *stmt = NULL;
	char pTempCmd[256];
	char *pErroMsg = NULL;
	sprintf(pTempCmd, "select * from bus_stu where bus_run_ID = \"%s\" and student_ID = \"%s\"", Bus_Student.bus_run_ID, Bus_Student.student_ID);
	if(SQLITE_OK != sqlite3_prepare(pDB, pTempCmd, strlen(pTempCmd), &stmt, NULL))
	{
		return FALSE;
	}

	if(SQLITE_ROW != sqlite3_step(stmt)) //没有数据，插入数据
	{
		sqlite3_finalize(stmt);
		sprintf(pTempCmd, "insert into bus_stu values (\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\")", Bus_Student.bus_run_ID, Bus_Student.student_ID, Bus_Student.up_station_ID, Bus_Student.up_time, Bus_Student.down_station_ID, Bus_Student.down_time, "");
		if( SQLITE_OK!=sqlite3_exec(pDB,pTempCmd,0,0,&pErroMsg))
		{
	#ifdef DEBUG_YFH
			CString error(pErroMsg);
			CString tmp;
			tmp = _T("输入表bus_student数据错误,错误原因:") + error;
			AfxMessageBox(tmp);
	#endif
			return FALSE; 
		}
 	}else{ //有数据，更新数据
		sqlite3_finalize(stmt);
		sprintf(pTempCmd, "update bus_stu set up_station_ID = \"%s\", up_time = \"%s\", down_station_ID = \"%s\", down_time = \"%s\" where bus_run_ID = \"%s\" and student_ID = \"%s\"", Bus_Student.up_station_ID, Bus_Student.up_time, Bus_Student.down_station_ID, Bus_Student.down_time, Bus_Student.bus_run_ID, Bus_Student.student_ID);
		if( SQLITE_OK!=sqlite3_exec(pDB,pTempCmd,0,0,&pErroMsg))
		{
	#ifdef DEBUG_YFH
			CString error(pErroMsg);
			CString tmp;
			tmp = _T("输入表bus_student数据错误1,错误原因:") + error;
			AfxMessageBox(tmp);
	#endif
			return FALSE; 
		}
	}
	return TRUE;
}

BOOL QueryBusDepart(char *bus_run_ID, struct bus_depart *bus_depart)
{
	sqlite3_stmt *stmt = NULL;
	char pTempCmd[256];
	char *pErroMsg = NULL;
	sprintf(pTempCmd, "select * from bus_depart where bus_run_ID = \"%s\"", bus_run_ID);

	sqlite3_prepare_v2(pDB, pTempCmd, strlen(pTempCmd), &stmt, NULL);

	while(sqlite3_step(stmt)  == SQLITE_ROW){
		char *name = (char *)sqlite3_column_text(stmt, 0);
		memcpy(bus_depart->bus_run_ID, name, strlen(name));

		name = (char *)sqlite3_column_text(stmt, 1);
		memcpy(bus_depart->bus_ID, name, strlen(name));

		name = (char *)sqlite3_column_text(stmt, 2);
		memcpy(bus_depart->driver_ID, name, strlen(name));

		name = (char *)sqlite3_column_text(stmt, 3);
		memcpy(bus_depart->teacher_ID, name, strlen(name));

		name = (char *)sqlite3_column_text(stmt, 4);
		memcpy(bus_depart->road_ID, name, strlen(name));

		name = (char *)sqlite3_column_text(stmt, 5);
		memcpy(bus_depart->student_number, name, strlen(name));
	}
	
	sqlite3_finalize(stmt);
	return TRUE;
}

BOOL QueryBusStu(char *bus_run_ID, char *student_ID, struct bus_stu *bus_stu)
{
	sqlite3_stmt *stmt = NULL;
	char pTempCmd[256];
	char *pErroMsg = NULL;
	sprintf(pTempCmd, "select * from bus_stu where bus_run_ID = \"%s\" and student_ID = \"%s\"", bus_run_ID, student_ID);

	sqlite3_prepare_v2(pDB, pTempCmd, strlen(pTempCmd), &stmt, NULL);

	while(sqlite3_step(stmt)  == SQLITE_ROW){
		char *name = (char *)sqlite3_column_text(stmt, 0);
		memcpy(bus_stu->bus_run_ID, name, strlen(name));

		name = (char *)sqlite3_column_text(stmt, 1);
		memcpy(bus_stu->student_ID, name, strlen(name));

		name = (char *)sqlite3_column_text(stmt, 2);
		memcpy(bus_stu->up_station_ID, name, strlen(name));

		name = (char *)sqlite3_column_text(stmt, 3);
		memcpy(bus_stu->up_time, name, strlen(name));

		name = (char *)sqlite3_column_text(stmt, 4);
		memcpy(bus_stu->down_station_ID, name, strlen(name));

		name = (char *)sqlite3_column_text(stmt, 5);
		memcpy(bus_stu->down_time, name, strlen(name));
	}
	
	sqlite3_finalize(stmt);
	return TRUE;
}


BOOL DeleteBusDepart(char *bus_run_ID)
{
	char pTempCmd[256];
	char *pTempOutputMsg = NULL;
	sprintf(pTempCmd, "delete from bus_depart where bus_run_ID=\"%d\"", bus_run_ID);
	if(SQLITE_OK != sqlite3_exec(pDB, pTempCmd, NULL, NULL, &pTempOutputMsg))
	{
#ifdef DEBUG_YFH
		CString error(pTempOutputMsg);
		CString tmp;
		tmp = _T("删除表bus_depart中数据错误,错误原因:") + error;
		AfxMessageBox(tmp);
#endif
		return FALSE; 
	}else
		return TRUE;
}

BOOL DeleteBusStu(char *bus_run_ID, char *student_ID)
{
	char pTempCmd[256];
	char *pTempOutputMsg = NULL;
	sprintf(pTempCmd, "delete from bus_stu where bus_run_ID=\"%d\" and and student_ID = \"%s\"", bus_run_ID, student_ID);
	if(SQLITE_OK != sqlite3_exec(pDB, pTempCmd, NULL, NULL, &pTempOutputMsg))
	{
#ifdef DEBUG_YFH
		CString error(pTempOutputMsg);
		CString tmp;
		tmp = _T("删除表bus_stu中数据错误,错误原因:") + error;
		AfxMessageBox(tmp);
#endif
		return FALSE; 
	}else
		return TRUE;
}

BOOL TestDatabase()
{
	struct bus_depart bus_depart;
	memset(&bus_depart, 0x00,sizeof(struct bus_depart));
	memcpy(bus_depart.bus_run_ID, "b12234", 6);
	memcpy(bus_depart.bus_ID, "bus_01", 6);
	memcpy(bus_depart.driver_ID, "driver_01", 9);
	memcpy(bus_depart.teacher_ID, "teacher_01", 10);
	memcpy(bus_depart.road_ID, "road_01", 7);
	memcpy(bus_depart.student_number, "256", 3);

	InsertBusDepart(bus_depart);
	
	memset(&bus_depart, 0x00, sizeof(struct bus_depart));
	QueryBusDepart("b12234", &bus_depart);
	return TRUE;	
}

BOOL InsertSchoolBus(StructPerson strtt, CString cp)
{
	UINT nret;
	for (int n=0;n<1;n++)
	{
		CString strSQL;
		strSQL.Format(L"insert into schoolbus values(NULL,'%s','%s','%s','%s','%s','%s','%s')",strtt.strName,strtt.strPhoneNum,strtt.strUpTime,strtt.strUpMessage,strtt.strDownTime,strtt.strDownMessage,cp);
		char *temp;
		char* errMsg;
		temp = (char *)(char*)(LPCTSTR)strSQL;	
		char temp_data[1024]={'\0'};
		WideCharToMultiByte(CP_ACP,0,(LPCWSTR)strSQL,-1,(LPSTR)temp_data,sizeof(temp_data),NULL,NULL);
		nret = sqlite3_exec(pDB,temp_data,0,0, &errMsg);
		if (nret != SQLITE_OK)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL UpdateSchoolBusUp(StructPerson sp)
{
	//下面更新数据库表中上车短信发送标示
	UINT i,nret;
	unsigned char *ctr;
	CString strSQL;
	wchar_t *tt= new wchar_t[50];
	char* errMsg;

	strSQL = _T("UPDATE schoolbus set IsUpMessageSended='是'");
	strSQL+=_T(" where StudentName='");
	strSQL+= sp.strName;
	strSQL+=_T("' and ParentPhoneNum='");
	strSQL+=sp.strPhoneNum;
	strSQL+=_T("' and UpBusTime='");
	strSQL+=sp.strUpTime;
	strSQL+=_T("'");

	char *temp;
	temp = (char *)(char*)(LPCTSTR)strSQL;	
	char temp_data[1024]={'\0'};
	WideCharToMultiByte(CP_ACP,0,(LPCWSTR)strSQL,-1,(LPSTR)temp_data,sizeof(temp_data),NULL,NULL);
	nret = sqlite3_exec(pDB,temp_data,0,0, &errMsg);
	if (nret != SQLITE_OK)
	{
	   AfxMessageBox(strSQL);	
	   return FALSE;
	}
	return TRUE;
}

BOOL UpdateSchoolBusDown(StructPerson sp, CString time)
{
	//下面更新数据库表中上车短信发送标示
	UINT i,nret;
	unsigned char *ctr;
	CString strSQL;
	wchar_t *tt= new wchar_t[50];
	char* errMsg;

	strSQL = _T("UPDATE schoolbus set IsDownMessageSended='是'");
	strSQL+=_T(" ,DownBusTime='");
	strSQL+= time;
	strSQL+=_T("' where StudentName='");
	strSQL+= sp.strName;
	strSQL+=_T("' and ParentPhoneNum='");
	strSQL+=sp.strPhoneNum;
	strSQL+=_T("' and UpBusTime='");
	strSQL+=sp.strUpTime;
	strSQL+=_T("'");

	char *temp;
	temp = (char *)(char*)(LPCTSTR)strSQL;	
	char temp_data[1024]={'\0'};
	WideCharToMultiByte(CP_ACP,0,(LPCWSTR)strSQL,-1,(LPSTR)temp_data,sizeof(temp_data),NULL,NULL);
	nret = sqlite3_exec(pDB,temp_data,0,0, &errMsg);
	if (nret != SQLITE_OK)
	{
		AfxMessageBox(strSQL);	
		return FALSE;
	}

	return TRUE;
}