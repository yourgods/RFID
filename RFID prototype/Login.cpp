// Login.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Login.h"
#include "RFID prototype.h"
#include "Sipapi.h"
#include "steganogram.h"
#include "tools.h"

static unsigned char rx_data[1024];
static OVERLAPPED m_osRead, m_osWrite; // �����ص���/д

//UART�����̺߳���
UINT CLogin::RFID_Thread(LPVOID lpParam){
	HANDLE hStr;
	DCB dcb;
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
		PurgeComm(hStr, PURGE_TXCLEAR|PURGE_RXCLEAR);
	}

	CLogin *pWnd = (CLogin *)lpParam;
	BOOL driver_read = FALSE;
	BOOL teacher_read = FALSE;
	BOOL bState = TRUE;
	DWORD length = 0;
	while(bState)
	{
		CString miyao,codetemp;
		char mingwen[28];
		unsigned int keybox[7][10];
		unsigned int dekeybox[7][10];
		ReadFile(hStr,rx_data,240,&length,&m_osRead); 
		codetemp=rx_data;
		miyao=LogisticAlgorithm();
		key(miyao,keybox);
		de_key(keybox,dekeybox);
		uncode(codetemp,mingwen,dekeybox);

		if(length)
		{
			//��ȡ��ʦ��������id�ź�������Ϣ

			//Got Phone No., student name & student ID no.
			//str1= mingwen;
			//CString strTemp,strTemp2;
			//char ctemp[34]={'\0'};			
			//for (int i=0;i<17;i++)
			//{
			//	ctemp[i*2] = (rx_data[i]>>4)&0x0F;
			//	ctemp[i*2+1] = rx_data[i]&0x0F;
			//}
			//char ch11[12];
			//for (int n=16;n<27;n++)
			//{
			//	ch11[n-16] = Hex2Char(ctemp[n]);
			//}
			//ch11[11] = '\0';
			//CString strPhone;
			//strPhone = ch11;
			////AfxMessageBox(strPhone);

			//char ch6[13];
			//for (int n=4;n<16;n++)
			//{
			//	ch6[n-4] = Hex2Char(ctemp[n]);
			//}	
			//ch6[12]='\0';

			//CString input;
			//input = ch6;
			//CString strName;
			//// CString to char*
			//int strlen1 = input.GetLength();
			//char* str = new char[strlen1+1];
			//memset(str, 0, strlen1+1);
			//for(int i=0; i<strlen1; i++)	str[i] = input.GetAt(i);
			//// hex string to wchar_t*
			//int wchslen = 0;
			//if(HexStr2WChars(str, strlen1, 4, NULL, &wchslen) != -1){
			//	wchar_t* wchs = new wchar_t[wchslen+1];
			//	memset(wchs, 0, sizeof(wchar_t)*(wchslen+1));
			//	int n = HexStr2WChars(str, strlen1, 4, wchs, &wchslen);
			//	if(n != -1){
			//		// wchar_t to CString
			//		wchar_t* pwch = wchs;
			//		for(int i=0; i<wchslen; i++)	strName.AppendChar(*pwch++);
			//	}
			//	if(wchs)	delete []wchs;
			//}

			////Read Student ID
			////....
			//CString strStudentID = _T("stud_01");
			CString strID, strName;
			//read id & name information
			if(strName.Compare(_T("teacher")) == 0){
				teacher_read = TRUE;
				pWnd->m_strTeacherID = strID;
				pWnd->m_strTeacherName = strName;
				pWnd->SetDlgItemText(IDC_EDIT2, strName);
			}else if(strName.Compare(_T("driver")) == 0){
				driver_read = TRUE;
				pWnd->m_strDriverID = strID;
				pWnd->m_strDriverName = strName;
				pWnd->SetDlgItemText(IDC_EDIT3, strName);
			}

		}
		// ����Ƿ��йرձ��̵߳��ź�
		DWORD dwEvent = WaitForSingleObject(pWnd->m_hKillThreadEvent, 20);
		if (dwEvent == WAIT_OBJECT_0)  bState = FALSE;
		Sleep(3); //����ʱ
	}

	// �ø��߳̽�����־
	SetEvent(pWnd->m_hThreadKilledEvent);
	CloseHandle(hStr);
	return 9999;
}

// CLogin �Ի���

IMPLEMENT_DYNAMIC(CLogin, CDialog)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CLogin::IDD, pParent)
	, m_strTeacherName(_T(""))
	, m_strTeacherID(_T(""))
	, m_strDriverName(_T(""))
	, m_strDriverID(_T(""))
	, m_strMsg(_T(""))
{
	m_hKillThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hThreadKilledEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	// �������߳�
	AfxBeginThread(RFID_Thread, this, THREAD_PRIORITY_NORMAL);
}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strTeacherName);
	DDX_Text(pDX, IDC_EDIT3, m_strDriverName);
	DDX_Text(pDX, IDC_MSG, m_strMsg);
}


BEGIN_MESSAGE_MAP(CLogin, CDialog)
	ON_BN_CLICKED(IDOK, &CLogin::OnBnClickedOk)
	ON_EN_SETFOCUS(IDC_EDIT2, &CLogin::OnEnSetfocusEdit2)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CLogin::OnEnKillfocusEdit2)
	ON_EN_SETFOCUS(IDC_EDIT3, &CLogin::OnEnSetfocusEdit3)
	ON_EN_KILLFOCUS(IDC_EDIT3, &CLogin::OnEnKillfocusEdit3)
	ON_BN_CLICKED(IDCANCEL, &CLogin::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLogin ��Ϣ�������

void CLogin::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(CheckPassword()){
		SetEvent(m_hKillThreadEvent);			// �����ر����̵߳��ź�
		WaitForSingleObject(m_hThreadKilledEvent, INFINITE);	// �ȴ����̹߳ر�

		CloseHandle(m_hKillThreadEvent);
		CloseHandle(m_hThreadKilledEvent);
		OnOK();
	}else{
		m_strMsg = _T("�û������������,������ˢ��!");
		SetDlgItemText(IDC_MSG, m_strMsg);
	}
}

BOOL CLogin::CheckPassword(void)
{
	//TODO: �ڴ�����ж������Ƿ���ȷ�ĳ������
	if(m_strTeacherName.GetLength() == 0)
		return FALSE;

	if(m_strTeacherID.GetLength() == 0)
		return FALSE;

	if(m_strDriverName.GetLength() == 0)
		return FALSE;

	if(m_strDriverID.GetLength() == 0)
		return FALSE;

	return TRUE;
}

void CLogin::OnEnSetfocusEdit2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SipShowIM(SIPF_ON);
}

void CLogin::OnEnKillfocusEdit2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SipShowIM(SIPF_OFF);
}

void CLogin::OnEnSetfocusEdit3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SipShowIM(SIPF_ON);
}

void CLogin::OnEnKillfocusEdit3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SipShowIM(SIPF_OFF);
}

void CLogin::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetEvent(m_hKillThreadEvent);			// �����ر����̵߳��ź�
	WaitForSingleObject(m_hThreadKilledEvent, INFINITE);	// �ȴ����̹߳ر�

	CloseHandle(m_hKillThreadEvent);
	CloseHandle(m_hThreadKilledEvent);
	OnCancel();
}
