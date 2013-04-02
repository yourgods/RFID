// RFID prototype.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

#include "config.h"
// CRFIDprototypeApp:
// �йش����ʵ�֣������ RFID prototype.cpp
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
	
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRFIDprototypeApp theApp;
