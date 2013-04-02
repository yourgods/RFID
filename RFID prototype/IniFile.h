// IniFile.h: interface for the CIniFile class.
// Written by: Gao Tong
// Email: martianfighter@yahoo.com.cn
// You may use this class/code as you wish in your programs.  Feel free to distribute it, and
// email suggested changes to me.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIFILE_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
#define AFX_INIFILE_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afx.h>
#include <afxtempl.h>
//#include <iostream.h>
#include "config.h"

class CIniFile  
{
	//all private variables
	
private:

	//stores path of ini file to read/write
	
	//stores name of ini file to read/write
	

	//get value(CString) from str
	CString GetValueString(CString str);

	//find Section in ini file
	BOOL FindSection(CString Section);

	//ini file
	CStdioFile IniFile;

	//public variables
public:
	CString m_strCP,m_strCount,m_strPhoneNum;
	CString m_strFileName;
	CString m_strFilePath;
	CString m_strBusID;
	CString m_strBusRunID;
	CString m_strRoadID;
	CString m_nStationNum;
	CArray<CString> m_cStations;
	CArray<CString> m_cStationName;
	//public functions


public:
	BOOL ReadIni();
	BOOL ReadStations();
	BOOL WriteIni();
	BOOL WriteIniBusRunID();
	//read value of int from ini file
	BOOL GetItemInt(CString Section,CString Item,int & Value);
	
	//read value of CString from ini file
	BOOL GetItemString(CString Section,CString Item,CString & Value);
	
	//write any string to ini file,"\n"
	BOOL WriteString(CString str);
	
	//write Section to ini file
	BOOL WriteSection(CString Section);
	
	//write Item and value of int to ini file
	BOOL WriteItemInt(CString Item,int Value);
	
	//write Item and value of CString to ini file
	BOOL WriteItemString(CString Item,CString Value);
	
	//open ini file for read
	BOOL OpenIniFileForRead(void);
	
	//open ini file for write
	BOOL OpenIniFileForWrite(void);
	
	//default constructor
	CIniFile();
	
	//sets name of ini file to read and write from
	void SetName(CString Name);
	
	//sets path of ini file to read and write from
	void SetPath(CString Path);
	
	//close ini file
	virtual void CloseIniFile(void);
	
	//default destructor
	virtual ~CIniFile();

};

#endif // !defined(AFX_INIFILE_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
