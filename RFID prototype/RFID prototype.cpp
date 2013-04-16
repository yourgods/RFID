// RFID prototype.cpp : ����Ӧ�ó��������Ϊ��
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


// CRFIDprototypeApp ����
CRFIDprototypeApp::CRFIDprototypeApp()
	: CWinApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_strPort=GSM_COM;
	m_strRate=_T("9600");
	m_strSmsc=SMSC;
	hStr=INVALID_HANDLE_VALUE;
}


// Ψһ��һ�� CRFIDprototypeApp ����
CRFIDprototypeApp theApp;

// CRFIDprototypeApp ��ʼ��

BOOL CRFIDprototypeApp::InitInstance()
{

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	if(!CheckPeripheral()){
		AfxMessageBox(L"У����ȫϵͳ�޷���������������ϵ����!");
		return FALSE;
	}
	CRFIDprototypeDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		// ��ȷ�������رնԻ���Ĵ���
		CloseGSM();
		CloseRFID();
		CloseGPS();
		CloseOthers();
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
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
		strError.Format(L"�޷��򿪶˿�%s! ����������?", m_strPort);
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
		strError.Format(L"�˿�%s��û�з���MODEM!", m_strPort);
		AfxMessageBox(strError, MB_OK);
		CloseComm();
		return FALSE;
	}

	if (m_strSmsc.IsEmpty())
	{
		AfxMessageBox(L"������SMSC!");
		 
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
		AfxMessageBox(L"�޷���RFID COM2:!");
		return FALSE;
	}
	else
	{
		if(!GetCommState(hStr, &dcb))
			return 0;
		dcb.BaudRate = 19200; // ���ݴ�������
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