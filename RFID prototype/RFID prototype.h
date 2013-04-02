// RFID prototype.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

#include "config.h"
// CRFIDprototypeApp:
// 有关此类的实现，请参阅 RFID prototype.cpp
//

class CRFIDprototypeApp : public CWinApp
{

//add by yfh 2013-2-27
public:
	CString m_strPort;
	CString m_strRate;
	CString m_strSmsc;
	HANDLE hStr;
	DCB dcb;

private:
	BOOL CheckPeripheral();
	BOOL CheckGSM();
	BOOL CheckRFID();
	BOOL CheckGPS();
	BOOL CheckOthers();
	BOOL CloseGSM();
	BOOL CloseRFID();
	BOOL CloseGPS();
	BOOL CloseOthers();

public:
	CRFIDprototypeApp();
	
// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CRFIDprototypeApp theApp;
