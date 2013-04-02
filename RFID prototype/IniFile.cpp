// IniFile.cpp: implementation of the CIniFile class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IniFile.h"
#include "config.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////

//default constructor
CIniFile::CIniFile()
{
}

//default destructor
CIniFile::~CIniFile()
{

}

/////////////////////////////////////////////////////////////////////
// Public Functions
/////////////////////////////////////////////////////////////////////

//sets path of ini file to read and write from
void CIniFile::SetPath(CString Path)
{
	m_strFilePath = Path;
}

//sets name of ini file to read and write from
void CIniFile::SetName(CString Name)
{
	m_strFileName=Name+_T(".ini");
}

//open ini file for read
BOOL CIniFile::OpenIniFileForRead()
{
	BOOL bExist=IniFile.Open(m_strFilePath+m_strFileName,CFile::modeRead); 
	return bExist;
}

//open ini file for write
BOOL CIniFile::OpenIniFileForWrite()
{
	BOOL bExist=IniFile.Open(m_strFilePath+m_strFileName,CFile::modeCreate | CFile::modeWrite);
	return bExist;
}

void CIniFile::CloseIniFile()
{
	IniFile.Close();
}

//find Section in ini file
BOOL CIniFile::FindSection(CString Section)
{
	IniFile.SeekToBegin();
	CString str;
	BOOL bFind=FALSE;
	BOOL bEnd=IniFile.ReadString(str);
	while(!bFind && bEnd)
	{	
		int index = str.Find(Section);
		if(index>=0)
		{
			bFind=TRUE;
			break;
		}
		bEnd=IniFile.ReadString(str);
	}
	return bFind;
}

//write Section to ini file
BOOL CIniFile::WriteSection(CString Section)
{
	CFileStatus status;
	if(IniFile.GetStatus(status))
	{
		IniFile.WriteString(_T("[")+Section+_T("]"));
		return TRUE;
	}
	else
		return FALSE;
}

//write Item and value of int to ini file
BOOL CIniFile::WriteItemInt(CString Item, int Value)
{
	CFileStatus status;
	if(IniFile.GetStatus(status))
	{
		CString str;
		str.Format(_T("%d"),Value);
		str=Item+_T("=")+str;
		IniFile.WriteString(str);
		return TRUE;
	}
	else
		return FALSE;
}

//write Item and value of CString to ini file
BOOL CIniFile::WriteItemString(CString Item, CString Value)
{
	CFileStatus status;
	if(IniFile.GetStatus(status))
	{
		IniFile.WriteString(Item+_T("=")+Value);
		return TRUE;
	}
	else
		return FALSE;
}

//read value of int from ini file
BOOL CIniFile::GetItemInt(CString Section, CString Item, int &Value)
{
	CFileStatus status;
	if(IniFile.GetStatus(status))
	{
		if(FindSection(Section))
		{
			CString buf,sub;
			BOOL bFind=FALSE;
			BOOL bEnd=IniFile.ReadString(buf);
			while(!bFind && bEnd)
			{
				sub=buf.Left(buf.Find(_T("=")));
				if(buf.Find(_T("["))<0 && !sub.IsEmpty() && sub.Find(Item)>=0)
				{
					sub=buf.Right(buf.GetLength()-buf.Find(_T("="))-1);
					sub=GetValueString(sub);
					Value=_ttoi(sub);
					bFind=TRUE;
					break;
				}
				bEnd=IniFile.ReadString(buf);
			}
			return bFind;
		}
		else
			return FALSE;
	}
	else
		return FALSE;
}

//read value of CString from ini file
BOOL CIniFile::GetItemString(CString Section, CString Item, CString &Value)
{
	CFileStatus status;
	if(IniFile.GetStatus(status))
	{
		if(FindSection(Section))
		{
			CString buf,sub;
			BOOL bFind=FALSE;
			BOOL bEnd=IniFile.ReadString(buf);
			while(!bFind && bEnd)
			{
				sub=buf.Left(buf.Find(_T("=")));
				if(buf.Find(_T("["))<0 && !sub.IsEmpty() && sub.Find(Item)>=0)
				{
					sub=buf.Right(buf.GetLength()-buf.Find(_T("="))-1);
					Value=GetValueString(sub);
					bFind=TRUE;
					break;
				}
				bEnd=IniFile.ReadString(buf);
			}
			return bFind;
		}
		else
			return FALSE;
	}
	else
		return FALSE;
}

//去掉字符串左右两边的空格
CString CIniFile::GetValueString(CString str)
{
	int length=str.GetLength();
	BOOL bSpace=TRUE;
	while(length && bSpace)
	{
		if(str[0]==' ')
			str=str.Right(--length);
		else
			bSpace=FALSE;
	}
	bSpace=TRUE;
	while(length && bSpace)
	{
		if(str[length-1]==' ')
			str=str.Left(--length);
		else
			bSpace=FALSE;
	}
	return str;
}

//将字符串写入ini文件中
BOOL CIniFile::WriteString(CString str)
{
	CFileStatus status;
	if(IniFile.GetStatus(status))
	{
		IniFile.WriteString(str);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CIniFile::ReadIni()
{
	//Creat or read ini file;
	CString str,path;
	SetName(INT_NAME);
	SetPath(INI_PATH);	
	
	BOOL bExist=OpenIniFileForRead();
	if(bExist)
	{
#ifndef DEBUG_YFH
		GetItemString(_T("PlateNumber"),_T("cp"),m_strCP);
		GetItemString(_T("MaxPerson"),_T("Count"),m_strCount);
		GetItemString(_T("SchoolNum"),_T("CenterPhone"),m_strPhoneNum);
		GetItemString(_T("BusID"), _T("id"), m_strBusID);
		GetItemString(_T("RoadID"), _T("id"), m_strRoadID);
		GetItemString(_T("BusRunID"), _T("id"), m_strBusRunID);
		ReadStations();
		CloseIniFile();
		return TRUE;
#else
		if (GetItemString(_T("PlateNumber"),_T("cp"),m_strCP)
			&&GetItemString(_T("MaxPerson"),_T("Count"),m_strCount)
			&&GetItemString(_T("SchoolNum"),_T("CenterPhone"),m_strPhoneNum)
			&&GetItemString(_T("BusID"), _T("id"), m_strBusID)
			&&GetItemString(_T("RoadID"), _T("id"), m_strRoadID)
			&&GetItemString(_T("BusRunID"), _T("id"), m_strBusRunID)
			&&ReadStations())
		{
			CloseIniFile();
			return TRUE;
		}
#endif
	}else{
		CString str;
		str.Format(_T("Error code:%d"), ::GetLastError());
		AfxMessageBox(str);
	}

	return FALSE;
}

BOOL CIniFile::ReadStations()
{
	int index = 0;
	int sum;
	GetItemString(_T("RoadStations"), _T("count"), m_nStationNum);
	sum = _ttoi(m_nStationNum);
	for(index = 0; index < sum; index++)
	{
		CString strIndex, strStation, strStationName;
		strIndex.Format(_T("%d"), index);
		GetItemString(_T("RoadStations"), strIndex, strStation);
		m_cStations.SetAtGrow(index, strStation);
		GetItemString(_T("StationName"), strIndex, strStationName);
		m_cStationName.SetAtGrow(index, strStationName);
	}
	return TRUE;
}

BOOL CIniFile::WriteIni()
{
	//Creat or read ini file;
	CString str,path;
	SetName(INT_NAME);
	SetPath(INI_PATH);	

#if 1
	BOOL bExist = OpenIniFileForRead();
	if(!bExist){
#endif
		BOOL bExist=OpenIniFileForWrite();
		if(bExist)
		{
			WriteSection(_T("PlateNumber"));
			WriteItemString(_T("cp"), _T("皖AP1234"));
			WriteSection(_T("MaxPerson"));
			WriteItemString(_T("Count"), _T("30"));
			WriteSection(_T("SchoolNum"));
			WriteItemString(_T("CenterPhone"), _T("18656563791"));
			WriteSection(_T("BusID"));
			WriteItemString(_T("id"), _T("bus_01"));
			WriteSection(_T("RoadID"));
			WriteItemString(_T("id"), _T("road_01"));
			WriteSection(_T("BusRunID"));
			WriteItemString(_T("id"), _T("123243"));
			WriteSection(_T("RoadStations"));
			WriteItemInt(_T("count"), 8);
			WriteItemString(_T("0"), _T("station_01"));
			WriteItemString(_T("1"), _T("station_02"));
			WriteItemString(_T("2"), _T("station_03"));
			WriteItemString(_T("3"), _T("station_04"));
			WriteItemString(_T("4"), _T("station_05"));
			WriteItemString(_T("5"), _T("station_06"));
			WriteItemString(_T("6"), _T("station_07"));
			WriteItemString(_T("7"), _T("station_08"));	
			WriteSection(_T("StationName"));
			WriteItemString(_T("0"), _T("站点1"));
			WriteItemString(_T("1"), _T("站点2"));
			WriteItemString(_T("2"), _T("站点3"));
			WriteItemString(_T("3"), _T("站点4"));
			WriteItemString(_T("4"), _T("站点5"));
			WriteItemString(_T("5"), _T("站点6"));
			WriteItemString(_T("6"), _T("站点7"));
			WriteItemString(_T("7"), _T("站点8"));
			CloseIniFile();
			return TRUE;
		}
#if 1
	}else{
		CloseIniFile();
		return TRUE;
	}
#endif
	return FALSE;
}

BOOL CIniFile::WriteIniBusRunID()
{
	//Creat or read ini file;
	CString str,path;
	SetName(INT_NAME);
	SetPath(INI_PATH);	
	
	BOOL bExist=OpenIniFileForWrite();
	if(bExist)
	{
		WriteSection(_T("PlateNumber"));
		WriteItemString(_T("cp"), m_strCP);
		WriteSection(_T("MaxPerson"));
		WriteItemString(_T("Count"), m_strCount);
		WriteSection(_T("SchoolNum"));
		WriteItemString(_T("CenterPhone"), m_strPhoneNum);
		WriteSection(_T("BusID"));
		WriteItemString(_T("id"), m_strBusID);
		WriteSection(_T("RoadID"));
		WriteItemString(_T("id"), m_strRoadID);
		WriteSection(_T("BusRunID"));
		int busRunID = _ttoi(m_strBusRunID);
		CString tmp;
		tmp.Format(_T("%d"), (busRunID +1));
		WriteItemString(_T("id"), tmp);
		WriteSection(_T("RoadStations"));
		WriteItemString(_T("count"), m_nStationNum);
		int count = _ttoi(m_nStationNum);
		for(int i = 0; i<count; i++){
			CString index;
			index.Format(_T("%d"), i);
			WriteItemString(index, m_cStations.GetAt(i));
		}
		WriteSection(_T("StationName"));
		for(int i = 0; i<count; i++){
			CString index;
			index.Format(_T("%d"), i);
			WriteItemString(index, m_cStationName.GetAt(i));
		}
		CloseIniFile();
		return TRUE;
	}
	return FALSE;
}