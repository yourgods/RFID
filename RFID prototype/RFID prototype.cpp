// RFID prototype.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "RFID prototype.h"
#include "RFID prototypeDlg.h"
#include "sms.h"
#include "comm.h"
#include "Setting.h"
#include "tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRFIDprototypeApp

BEGIN_MESSAGE_MAP(CRFIDprototypeApp, CWinApp)
END_MESSAGE_MAP()


// CRFIDprototypeApp 构造
CRFIDprototypeApp::CRFIDprototypeApp()
	: CWinApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_strPort=GSM_COM;
	m_strRate=_T("9600");
	m_strSmsc=SMSC;
	hStr=INVALID_HANDLE_VALUE;
}


// 唯一的一个 CRFIDprototypeApp 对象
CRFIDprototypeApp theApp;

// CRFIDprototypeApp 初始化

BOOL CRFIDprototypeApp::InitInstance()
{

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	if(!CheckPeripheral()){
		AfxMessageBox(L"校车安全系统无法正常启动，请联系厂商!");
		return FALSE;
	}
	CRFIDprototypeDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		// “确定”来关闭对话框的代码
		CloseGSM();
		CloseRFID();
		CloseGPS();
		CloseOthers();
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

BOOL CRFIDprototypeApp::CloseGSM()
{
	gsmDeinit();
	CloseComm();
	return TRUE;
}

BOOL CRFIDprototypeApp::CloseRFID()
{
	CloseHandle(hStr);	
	return TRUE;
}

BOOL CRFIDprototypeApp::CloseGPS()
{
	return TRUE;
}

BOOL CRFIDprototypeApp::CloseOthers()
{
	return TRUE;
}

BOOL CRFIDprototypeApp::CheckPeripheral()
{
#ifdef DEBUG_YFH
	if(!CheckGSM())
		return FALSE;
	if(!CheckRFID())
		return FALSE;
	if(!CheckGPS())
		return FALSE;
	if(!CheckOthers())
		return FALSE;
#else
	CheckGSM();
	CheckRFID();
	CheckGPS();
	CheckOthers();
#endif
	return TRUE;
}

BOOL CRFIDprototypeApp::CheckGSM()
{
	int nRate;
	nRate = _ttoi(m_strRate);

	while (!::OpenComm(m_strPort, nRate))
	{
		CString strError;
		strError.Format(L"无法打开端口%s! 现在设置吗?", m_strPort);
		if (AfxMessageBox(strError, MB_YESNO) == IDNO) return FALSE;

		CSetting dlg;
		dlg.m_strPort = m_strPort;
		dlg.m_strRate = m_strRate;
		dlg.m_strSmsc = m_strSmsc;
		if (dlg.DoModal() == IDOK)
		{
			m_strPort = dlg.m_strPort;
			m_strRate = dlg.m_strRate;
			m_strSmsc = dlg.m_strSmsc;
		
		}
		else
		{
			return FALSE;
		}
	}

	if (!gsmInit())
	{
		CString strError;
		strError.Format(L"端口%s上没有发现MODEM!", m_strPort);
		AfxMessageBox(strError, MB_OK);
		CloseComm();
		return FALSE;
	}

	if (m_strSmsc.IsEmpty())
	{
		AfxMessageBox(L"请设置SMSC!");
		 
 		CSetting dlg;
 		dlg.m_strPort = m_strPort;
 		dlg.m_strRate = m_strRate;
 		dlg.m_strSmsc = m_strSmsc;
 		if (dlg.DoModal() == IDOK)
 		{
 			m_strPort = dlg.m_strPort;
 			m_strRate = dlg.m_strRate;
 			m_strSmsc = dlg.m_strSmsc;
 		}
 		else
 		{
			CloseComm();
 			return FALSE;
 		}
	}

	return TRUE;
}

BOOL CRFIDprototypeApp::CheckRFID()
{
	hStr = CreateFile(RFID_COM, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, 0, 0);
	if (INVALID_HANDLE_VALUE == hStr)
	{
		AfxMessageBox(L"无法打开RFID COM2:!");
		return FALSE;
	}
	else
	{
		if(!GetCommState(hStr, &dcb))
			return 0;
		dcb.BaudRate = 19200; // 数据传输速率
		SetCommState(hStr, &dcb);
	}
	return TRUE;
}

BOOL CRFIDprototypeApp::CheckGPS()
{
	return TRUE;
}

BOOL CRFIDprototypeApp::CheckOthers(){
	return TRUE;
}