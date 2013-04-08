// RFID prototypeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RFID prototype.h"
#include "RFID prototypeDlg.h"
#include "Login.h"
#include "sqlite3.h"
#include "config.h"
#include "tools.h"
#include "sms.h"
#include "RFID.h"
#include "database.h"
#include "receive.h"
#include "message.h"
#include "SmsTrafic.h"
#include <aygshell.h>

#pragma comment(lib,"sqlite3.lib")
#pragma comment(lib,"aygshell.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE hReadThread = INVALID_HANDLE_VALUE;
DWORD dwTStat;
CIniFile iniFile;
int noStation = 0;

CArray<StructPerson,StructPerson> strArray;

// CRFIDprototypeDlg �Ի���
CRFIDprototypeDlg::CRFIDprototypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRFIDprototypeDlg::IDD, pParent)
	, m_strDriverID(_T(""))
	, m_strTeacherID(_T(""))
	, m_strCurrentStation(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strSmsc = SMSC;
	gsm = NULL;
}

void CRFIDprototypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListPassenger);
}

BEGIN_MESSAGE_MAP(CRFIDprototypeDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON5, &CRFIDprototypeDlg::OnBnClickedButton5)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CRFIDprototypeDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CRFIDprototypeDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CRFIDprototypeDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CRFIDprototypeDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &CRFIDprototypeDlg::OnBnClickedButton6)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON7, &CRFIDprototypeDlg::OnBnClickedButton7)
END_MESSAGE_MAP()


// CRFIDprototypeDlg ��Ϣ�������

BOOL CRFIDprototypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//����ͼƬ
	m_BackScreenBitmap.LoadBitmap(IDB_BKG);

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//����������Ϊȫ��������������
#ifndef DEBUG_YFH
	HWND hTask = ::FindWindow(TEXT("HHTaskBar"), NULL);
	if (hTask)
	{
	::ShowWindow(hTask, SW_HIDE);
	}
	DWORD dwState = SHFS_HIDETASKBAR | SHFS_HIDESTARTICON | SHFS_HIDESIPBUTTON;
	SHFullScreen(this->GetSafeHwnd(), dwState);
	::MoveWindow(this->GetSafeHwnd(), 0,0,GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), TRUE); 
#endif
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	// ��½У����ȫϵͳ����ȡ�泵��ʦ��˾������
	CLogin loginDlg;
	if (loginDlg.DoModal() == IDOK)
	{
#ifdef DEBUG_YFH
		//::MessageBox(NULL, _T("��½�ɹ�"), _T("��½"), MB_OK);
#endif
		m_strTeacherID = loginDlg.m_strName;
		m_strDriverID = DRIVER_ID;
	}else{
		::MessageBox(NULL, _T("��½ʧ��,�˳�ϵͳ!"), _T("��½"), MB_OK);
		OnOK();
	}

	//��ini�ļ��ж�ȡ����,���ݰ���:���ƺ�,У�����,�����α��,ѧУ�绰����,���������,·����ż���վ����
	iniFile.WriteIni();
	if(iniFile.ReadIni())
	{
		////��ʾ������ini�ļ�
		//CString strIni;
		//strIni += _T("���ƺ�:");
		//strIni += iniFile.m_strCP;
		//strIni += _T("\r\n���������:");
		//strIni += iniFile.m_strCount;
		//strIni += _T("\r\nѧУ����:");
		//strIni += iniFile.m_strPhoneNum;
		//strIni += _T("\r\nУ�����:");
		//strIni += iniFile.m_strBusID;
		//strIni += _T("\r\n·�����:");
		//strIni += iniFile.m_strRoadID;
		//strIni += _T("\r\n�����α��:");
		//strIni += iniFile.m_strBusRunID;
		//::MessageBox(NULL, strIni, _T("����"), MB_OK);
	}else{
		::MessageBox(NULL, _T("�޷����������ļ�,��ʹ��Ĭ������!"), _T("����"), MB_OK);
	}	

	gsm = new CSmsTraffic();
	if(gsm == NULL){
		::MessageBox(NULL, _T("�޷�����gsm�豸,����ϵ�豸�ṩ��!"), _T("����"), MB_OK);
		OnOK();
	}

	//��ʼ��List control
	InitList();

	//��ȡ���ݿ�(doing)
	CheckDataBaseExist();

	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);

	iniFile.WriteIniBusRunID();
	noStation = 0;
	m_strCurrentStation = iniFile.m_cStations.GetAt(noStation);

	//����RFIDģ�飬���ж�������
	//���������̡߳�
	if (!GetExitCodeThread(hReadThread, &dwTStat) ||(dwTStat != STILL_ACTIVE)) 
	{
		hReadThread = CreateThread (NULL, 0,RXINT_Thread, this,0, &dwTStat);
	}
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CRFIDprototypeDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_RFIDPROTOTYPE_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_RFIDPROTOTYPE_DIALOG));
	}
}
#endif


void CRFIDprototypeDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strContent,strPhoneEnd,strCenterNum,strUpTime,strDownTime;

	CString strTempStart;
	strTempStart = "86";
	int  NumUp = 0,NumDown = 0;
	strCenterNum = SMSC;

	//�ϳ�����
	CString strTemp2;	
	NumUp= strArray.GetCount();
	strTemp2.Format(_T("%d"),NumUp);

	//���³�ʱ��
	strUpTime = m_ListPassenger.GetItemText(0,2);
	strDownTime = m_ListPassenger.GetItemText(NumUp - 1,4);

	//����������
	strPhoneEnd = strTempStart + iniFile.m_strPhoneNum;
	
	//�³�����
	StructPerson stPerson;
	for (int j=0;j<strArray.GetCount();j++)
	{
		stPerson = strArray.GetAt(j);
		if(stPerson.strDownMessage == "��")
		{
			NumDown++;
		}
	}

	CString strTemp;
	strTemp.Format(_T("%d"),NumDown);
	strContent= iniFile.m_strCP;
	strContent+=_T(",");
	strContent+=strUpTime;
	strContent+=_T(",");
	strContent+=strTemp2;
	strContent+=_T(",");
	strContent+=strDownTime;
	strContent+=_T(",");
	strContent+=strTemp;
	strContent+=_T(",");
	SM_PARAM sm_param;
	USES_CONVERSION;
	CHAR* pch=W2A(strContent);
	strcpy(sm_param.TP_UD, pch);

	pch=W2A(strPhoneEnd);
	strcpy(sm_param.TPA, pch);

	strCenterNum = SMSC;
	pch=W2A(strCenterNum);
	strcpy(sm_param.SCA, pch);

	sm_param.TP_PID = 0;
	sm_param.TP_DCS = GSM_UCS2;

#ifdef USE_TRAFFIC
	gsm->PutSendMessage(&sm_param);
#else
	::gsmSendMessage(&sm_param);
#endif

	if(NumUp!=NumDown){
	//	PlaySound(L"\\ResidentFlash\\Sound\\Warning.wav",NULL,SND_SYNC|SND_NODEFAULT);
		::MessageBox(NULL, _T("\\ResidentFlash\\Sound\\Warning.wav"), _T("���³���������!"), MB_OK);
	}
	stPerson.strDownMessage="";
	stPerson.strDownTime="";
	stPerson.strName="";
	stPerson.strPhoneNum="";
	stPerson.strUpMessage="";
	stPerson.strUpTime="";
	strArray.RemoveAll();

	m_ListPassenger.DeleteAllItems();
	SetDlgItemText(IDC_STATIC_PASSANGERNUM,L"0");
	CloseDatabase();
	KillTimer(TIMER);
	if(gsm != NULL)
		delete gsm;
	noStation = 0;
	OnOK();
}

void CRFIDprototypeDlg::InitList(void)
{
	//�����б��ʼ��
	CRect rect;
	GetWindowRect(&rect);
	
	((CListCtrl *)GetDlgItem(IDC_LIST1))->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	((CListCtrl *)GetDlgItem(IDC_LIST1))->InsertColumn(0,_T("����"),LVCFMT_LEFT,80);
 	((CListCtrl *)GetDlgItem(IDC_LIST1))->InsertColumn(1,_T("�ҳ���ϵ��ʽ"),LVCFMT_LEFT,120);
 	((CListCtrl *)GetDlgItem(IDC_LIST1))->InsertColumn(3,_T("�ϳ�ʱ��"),LVCFMT_LEFT,170);
 	((CListCtrl *)GetDlgItem(IDC_LIST1))->InsertColumn(4,_T("�ϳ�����"),LVCFMT_LEFT,80);
 	((CListCtrl *)GetDlgItem(IDC_LIST1))->InsertColumn(5,_T("�³�ʱ��"),LVCFMT_LEFT,170);
 	((CListCtrl *)GetDlgItem(IDC_LIST1))->InsertColumn(6,_T("�³�����"),LVCFMT_LEFT,80);
 	((CListCtrl *)GetDlgItem(IDC_LIST1))->InsertColumn(7,_T("���ƺ�"),LVCFMT_LEFT,80);
 	((CListCtrl *)GetDlgItem(IDC_LIST1))->InsertColumn(8,_T("ID"),LVCFMT_LEFT,0);
}

void CRFIDprototypeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	//��վ����
	CRect rect; 
	GetClientRect(&rect); 
	CDC dcMem; 
	dcMem.CreateCompatibleDC(&dc); 
	//BITMAP bitmap; 
	//m_BackScreenBitmap.GetBitmap(&bitmap); 
	//CBitmap *pbmpOld=dcMem.SelectObject(&m_BackScreenBitmap); 
	//dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0, bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 

	CPen *pen = (CPen *)dc.SelectStockObject(WHITE_PEN);
	int start = 50;
	int length = rect.Width()-100;
	dc.MoveTo(start, 80);
	dc.LineTo(length + start,80);
	FlushPoint(&dc, noStation, start, length, 80);
}

void CRFIDprototypeDlg::FlushPoint(CPaintDC *dc, int index, int start, int length, int y){
	int i = 0, radius = 5;
	int stations = _ttoi(iniFile.m_nStationNum);
	int step = length/(stations-1);
	CBrush brushGreen(RGB(0,255,0));
	CBrush *pOldBrush = dc->SelectObject(&brushGreen);
	CPen *pen = (CPen *)dc->SelectStockObject(WHITE_PEN);
	if(index >= stations)
		index = stations - 1;
	while(i<stations){
		if(i == index){
			CBrush brushRed(RGB(255, 0, 0));
			CBrush *pOldBrush2 = dc->SelectObject(&brushRed);
			CRect rect((start + i*step)-radius, y-radius, (start + i*step)+radius, y+radius);
			dc->Ellipse(&rect);
			dc->SelectObject(pOldBrush2);
			brushRed.DeleteObject();
		}else{
			CRect rect((start + i*step)-radius, y-radius, (start + i*step)+radius, y+radius);
			dc->Ellipse(&rect);
		}
		if(i%2 == 0){
			dc->ExtTextOut((start+i*step)-5*radius,y+2*radius, ETO_CLIPPED, NULL, iniFile.m_cStationName.GetAt(i), NULL);
		}else{
			dc->ExtTextOut((start+i*step)-5*radius,y-5*radius, ETO_CLIPPED, NULL, iniFile.m_cStationName.GetAt(i), NULL);		
		}
		i++;
	}
	dc->SelectObject(pOldBrush);
	dc->SelectObject(pen);
	brushGreen.DeleteObject();
}


void CRFIDprototypeDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	//��ʾ���ص�������
	HWND hTask = ::FindWindow(TEXT("HHTaskBar"), NULL);
	if (hTask)
	{
	::ShowWindow(hTask, SW_SHOW);
	}
	CDialog::OnCancel();
}

void CRFIDprototypeDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	//��ʾ���ص�������
	HWND hTask = ::FindWindow(TEXT("HHTaskBar"), NULL);
	if (hTask)
	{
	::ShowWindow(hTask, SW_SHOW);
	}
	CDialog::OnOK();
}

void CRFIDprototypeDlg::UpdateList(void)
{
	StructPerson strtt;
	CString strName,strPhoneNum,strUpMessageSend,strUpBusTime,strDownBusTime,strBusCode,strDownMessageSend;

	m_ListPassenger.DeleteAllItems();
	for (int j=0;j<strArray.GetCount();j++)
	{
		strtt = strArray.GetAt(j);
		strName = strtt.strName;
		strPhoneNum = strtt.strPhoneNum;

		m_ListPassenger.InsertItem(j,(LPCTSTR)strtt.strName);
		m_ListPassenger.SetItemText(j, 1, (LPCTSTR)strtt.strPhoneNum);
		m_ListPassenger.SetItemText(j, 2, (LPCTSTR)strtt.strUpTime);
		m_ListPassenger.SetItemText(j, 3, (LPCTSTR)strtt.strUpMessage);
		m_ListPassenger.SetItemText(j, 4, (LPCTSTR)strtt.strDownTime);		
		m_ListPassenger.SetItemText(j, 5, (LPCTSTR)strtt.strDownMessage);
		m_ListPassenger.SetItemText(j, 6, (LPCTSTR)iniFile.m_strCP);
	}
}

// ����TRUE��˵���б����Ѿ����ڸ���Ϣ��˵�����³������FALSE˵�����ϳ���Ϣ
BOOL CRFIDprototypeDlg::CheckDataExist(CString strName, CString strPhone)
{
	CString strTemp1,strTemp2;
	for(int i=0;i<m_ListPassenger.GetItemCount();i++)
	{
		strTemp1 = m_ListPassenger.GetItemText(i,0);
		strTemp2 = m_ListPassenger.GetItemText(i,1);
		if (strTemp1==strName&&strTemp2==strPhone)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CRFIDprototypeDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//////////////////////////////////////////////////////////////////////////
    //���Բ�������
	//PlaySound(L"\\ResidentFlash\\Sound\\close.wav",NULL,SND_SYNC|SND_NODEFAULT);
	//////////////////////////////////////////////////////////////////////////

	//�ѷ�������Ϣ�������ݿ���
	struct bus_depart bus_depart;
	bus_depart.bus_run_ID = iniFile.m_strBusRunID;
	bus_depart.bus_ID = iniFile.m_strBusID;
	bus_depart.driver_ID = m_strDriverID;
	bus_depart.teacher_ID = m_strTeacherID;
	bus_depart.road_ID = iniFile.m_strRoadID;
	bus_depart.student_number = iniFile.m_strCount;
	bus_depart.remark = _T("");
	InsertBusDepart(bus_depart);

	//�ѷ�������Ϣ���͵�������
	CString strContent, strPhoneEnd, strCenterNum;
	strContent = _T("1,");
	strContent += bus_depart.bus_run_ID;
	strContent += _T(",");
	strContent += bus_depart.bus_ID;
	strContent += _T(",");
	strContent += bus_depart.driver_ID;
	strContent += _T(",");
	strContent += bus_depart.teacher_ID;
	strContent += _T(",");
	strContent += bus_depart.road_ID;
	strContent += _T(",");
	strContent += bus_depart.student_number;
	strContent += _T(",");

	SM_PARAM sm_param;
	USES_CONVERSION;
	CHAR* pch=W2A(strContent);
	strcpy(sm_param.TP_UD, pch);

	strPhoneEnd = _T("86") + iniFile.m_strPhoneNum;
	pch=W2A(strPhoneEnd);
	strcpy(sm_param.TPA, pch);

	strCenterNum = m_strSmsc;
	pch=W2A(strCenterNum);
	strcpy(sm_param.SCA, pch);

	sm_param.TP_PID = 0;
	sm_param.TP_DCS = GSM_8BIT;

#ifdef USE_TRAFFIC
	gsm->PutSendMessage(&sm_param);
#else
	::gsmSendMessage(&sm_param);
#endif

	//if(m_ListPassenger.GetItemCount()<=0)
	//{
	//	SetTimer(TIMER, ELAPSE, 0);
	//	return ;
	//}

	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);

	OnBnClickedButton3();
	OnBnClickedButton2();
	
	//StructPerson strtt;
	//CString strName,strPhoneNum,strUpMessageSend,strUpBusTime,strDownBusTime,strBusCode,strDownMessageSend;
	//for (int j=0;j<strArray.GetCount();j++)
	//{
	//	strtt = strArray.GetAt(j);

	//	InsertSchoolBus(strtt, iniFile.m_strCP);

	//	//��У��ѧ����Ϣ�洢�����ݿ���
	//	struct bus_stu bus_stu;
	//	bus_stu.bus_run_ID = iniFile.m_strBusRunID;
	//	bus_stu.student_ID = strtt.strID;
	//	bus_stu.up_station_ID = strtt.strUpStation;
	//	bus_stu.up_time = strtt.strUpTime;
	//	bus_stu.down_station_ID = strtt.strDownStation;
	//	bus_stu.down_time = strtt.strDownTime;
	//	bus_stu.remark = _T("");
	//	InsertBusStu(bus_stu);

	//	if(strtt.strDownMessage.Compare(_T("��")) == 0 && strtt.strUpMessage.Compare(_T("��")) == 0)
	//		continue;

	//	if((!strtt.strDownTime.IsEmpty())&&(strtt.strDownMessage.Compare(_T("��")) == 0))
	//		strtt.strDownMessage = _T("��");
	//	if((!strtt.strUpTime.IsEmpty())&&(strtt.strUpMessage.Compare(_T("��")) == 0))
	//		strtt.strUpMessage = _T("��");	

	//	strArray.SetAt(j, strtt);
	//	//����У��ѧ����Ϣ����������
	//	strContent = _T("3,");
	//	strContent += bus_stu.bus_run_ID;
	//	strContent += _T(",");
	//	strContent += bus_stu.student_ID;
	//	strContent += _T(",");
	//	strContent += bus_stu.up_station_ID;
	//	strContent += _T(",");
	//	strContent += bus_stu.up_time;
	//	strContent += _T(",");
	//	strContent += bus_stu.down_station_ID;
	//	strContent += _T(",");
	//	strContent += bus_stu.down_time;
	//	strContent += _T(",");
	//	strContent += bus_stu.remark;
	//	SM_PARAM sm_param;
	//	USES_CONVERSION;
	//	CHAR* pch=W2A(strContent);
	//	strcpy(sm_param.TP_UD, pch);

	//	strPhoneEnd = _T("86") + iniFile.m_strPhoneNum;
	//	pch=W2A(strPhoneEnd);
	//	strcpy(sm_param.TPA, pch);

	//	strCenterNum = m_strSmsc;
	//	pch=W2A(strCenterNum);
	//	strcpy(sm_param.SCA, pch);

	//	sm_param.TP_PID = 0;
	//	sm_param.TP_DCS = GSM_8BIT;

	//#ifdef USE_TRAFFIC
	//	gsm->PutSendMessage(&sm_param);
	//#else
	//	::gsmSendMessage(&sm_param);
	//#endif
	//}

	SetTimer(TIMER, ELAPSE, 0);
}

void CRFIDprototypeDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//AfxMessageBox(L"�ϳ�����");
	CString strName,strContent,strendContent,strPhone,strPhoneEnd,strCenterNum,strUpTime;
	strCenterNum = SMSC;
	CString strTempStart;
	strTempStart = "86";
	strendContent = "����С�����ϳ������ƺ�Ϊ��";
	strendContent += iniFile.m_strCP;
	CString strSendFlag;

	//ergodic each student
	for (int n=0;n<m_ListPassenger.GetItemCount();n++)
	{
		strName = m_ListPassenger.GetItemText(n,0);
		strPhone = m_ListPassenger.GetItemText(n,1);
		strUpTime = m_ListPassenger.GetItemText(n,2);
		strSendFlag = m_ListPassenger.GetItemText(n,3);

		if (strSendFlag=="��")
		{
			//send upstation message to the student's parent
			strContent = "�𾴵�";
			strContent+=strName;
			strContent+="�ҳ���";

			strContent = strContent+strendContent;
			strPhoneEnd = strTempStart+strPhone;

			SM_PARAM sm_param;
			USES_CONVERSION;
			CHAR* pch=W2A(strContent);
			strcpy(sm_param.TP_UD, pch);

			pch=W2A(strPhoneEnd);
			strcpy(sm_param.TPA, pch);

			pch=W2A(strCenterNum);
			strcpy(sm_param.SCA, pch);

			sm_param.TP_PID = 0;
			sm_param.TP_DCS = GSM_UCS2;
#ifdef USE_TRAFFIC
			gsm->PutSendMessage(&sm_param);
#else
			::gsmSendMessage(&sm_param);
#endif

			StructPerson stPerson;
			for (int j=0;j<strArray.GetCount();j++)
			{
				stPerson = strArray.GetAt(j);
				if (stPerson.strName==strName&&stPerson.strPhoneNum==strPhone&&stPerson.strUpTime==strUpTime)
				{
					stPerson.strUpMessage = "��";
					strArray.SetAt(j,stPerson);
					UpdateSchoolBusUp(stPerson);

					//��У��ѧ����Ϣ�洢�ڱ������ݿ���
					struct bus_stu bus_stu;
					bus_stu.bus_run_ID = iniFile.m_strBusRunID;
					bus_stu.student_ID = stPerson.strID;
					bus_stu.up_station_ID = stPerson.strUpStation;
					bus_stu.up_time = stPerson.strUpTime;
					bus_stu.down_station_ID = stPerson.strDownStation;
					bus_stu.down_time = stPerson.strDownTime;
					bus_stu.remark = _T("");
					InsertBusStu(bus_stu);

					//����У��ѧ����Ϣ����������
					strContent = _T("3,");
					strContent += bus_stu.bus_run_ID;
					strContent += _T(",");
					strContent += bus_stu.student_ID;
					strContent += _T(",");
					strContent += bus_stu.up_station_ID;
					strContent += _T(",");
					strContent += bus_stu.up_time;
					strContent += _T(",");
					//strContent += bus_stu.down_station_ID;
					strContent += _T(",");
					//strContent += bus_stu.down_time;
					strContent += _T(",");
					strContent += bus_stu.remark;
					SM_PARAM sm_param;
					USES_CONVERSION;
					CHAR* pch=W2A(strContent);
					strcpy(sm_param.TP_UD, pch);

					strPhoneEnd = _T("86") + iniFile.m_strPhoneNum;
					pch=W2A(strPhoneEnd);
					strcpy(sm_param.TPA, pch);

					strCenterNum = m_strSmsc;
					pch=W2A(strCenterNum);
					strcpy(sm_param.SCA, pch);

					sm_param.TP_PID = 0;
					sm_param.TP_DCS = GSM_8BIT;

				#ifdef USE_TRAFFIC
					gsm->PutSendMessage(&sm_param);
				#else
					::gsmSendMessage(&sm_param);
				#endif
					break;

				}
			}
		}
	}	
	UpdateList();
}

void CRFIDprototypeDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strName,strContent,strendContent,strPhone,strPhoneEnd,strCenterNum,strUpTime,strDownTime;
	strCenterNum = SMSC;
	CString strTempStart;
	strTempStart = "86";
	strendContent = "����С�����³������ƺ�Ϊ��";
	strendContent+= iniFile.m_strCP;
	CString strSendFlag;
	for (int n=0;n<m_ListPassenger.GetItemCount();n++)
	{
		strName = m_ListPassenger.GetItemText(n,0);
		strPhone = m_ListPassenger.GetItemText(n,1);
		strUpTime = m_ListPassenger.GetItemText(n,2);
		strSendFlag = m_ListPassenger.GetItemText(n,5);
		strDownTime = m_ListPassenger.GetItemText(n,4);

		if (strSendFlag=="��"&&strDownTime.Trim()!="")
		{
			strContent = "�𾴵�";
			strContent+=strName;
			strContent+="��";

			strContent = strContent+strendContent;
			strPhoneEnd = strTempStart+strPhone;

			SM_PARAM sm_param;
			USES_CONVERSION;
			CHAR* pch=W2A(strContent);
			strcpy(sm_param.TP_UD, pch);

			pch=W2A(strPhoneEnd);
			strcpy(sm_param.TPA, pch);

			pch=W2A(strCenterNum);
			strcpy(sm_param.SCA, pch);

			sm_param.TP_PID = 0;
			sm_param.TP_DCS = GSM_UCS2;

#ifdef USE_TRAFFIC
			gsm->PutSendMessage(&sm_param);
#else
			::gsmSendMessage(&sm_param);
#endif

			StructPerson stPerson;
			for (int j=0;j<strArray.GetCount();j++)
			{
				stPerson = strArray.GetAt(j);
				if (stPerson.strName==strName&&stPerson.strPhoneNum==strPhone&&stPerson.strUpTime==strUpTime)
				{
					stPerson.strDownMessage = "��";
					strArray.SetAt(j,stPerson);
					//CTime Downt=CTime::GetCurrentTime();
					//CString strDownTime = Downt.Format(L"%Y-%m-%d %H:%M:%S");
					UpdateSchoolBusDown(stPerson, stPerson.strDownTime);

					//�������ݿ���ѧ�����³���ʱ��վ����Ϣ
					struct bus_stu bus_stu;
					bus_stu.bus_run_ID = iniFile.m_strBusRunID;
					bus_stu.student_ID = stPerson.strID;
					bus_stu.up_station_ID = stPerson.strUpStation;
					bus_stu.up_time = stPerson.strUpTime;
					bus_stu.down_station_ID = stPerson.strDownStation;
					bus_stu.down_time = stPerson.strDownTime;
					bus_stu.remark = _T("");
					InsertBusStu(bus_stu);

					//����У��ѧ����Ϣ����������
					strContent = _T("3,");
					strContent += bus_stu.bus_run_ID;
					strContent += _T(",");
					strContent += bus_stu.student_ID;
					strContent += _T(",");
					strContent += bus_stu.up_station_ID;
					strContent += _T(",");
					strContent += bus_stu.up_time;
					strContent += _T(",");
					strContent += bus_stu.down_station_ID;
					strContent += _T(",");
					strContent += bus_stu.down_time;
					strContent += _T(",");
					strContent += bus_stu.remark;	
					SM_PARAM sm_param;
					USES_CONVERSION;
					CHAR* pch=W2A(strContent);
					strcpy(sm_param.TP_UD, pch);

					strPhoneEnd = _T("86") + iniFile.m_strPhoneNum;
					pch=W2A(strPhoneEnd);
					strcpy(sm_param.TPA, pch);

					strCenterNum = m_strSmsc;
					pch=W2A(strCenterNum);
					strcpy(sm_param.SCA, pch);

					sm_param.TP_PID = 0;
					sm_param.TP_DCS = GSM_8BIT;

				#ifdef USE_TRAFFIC
					gsm->PutSendMessage(&sm_param);
				#else
					::gsmSendMessage(&sm_param);
				#endif
					break;
				}
			}
		}
	}	

	UpdateList();
}

void CRFIDprototypeDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
#ifdef USE_TRAFFIC
	CReceive dlg(this);
#else
	CReceive dlg;
#endif
	dlg.DoModal();
}

void CRFIDprototypeDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	noStation++;
	m_strCurrentStation = iniFile.m_cStations.GetAt(noStation);
	//����վ������
	CString strSoundPath;
	strSoundPath.Format(_T("\\ResidentFlash\\Sound\\Station%d.wav"), noStation);
	//PlaySound(strSoundPath,NULL,SND_SYNC|SND_NODEFAULT);
	::MessageBox(NULL, _T("����վ������:") + strSoundPath, _T("Station"), MB_OK);
	Invalidate();
}

void CRFIDprototypeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent){
		case TIMER:
			 //AfxMessageBox(_T("��ʱ��1"));
			SendStatusInfo();
			break;
		default:
			break;
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CRFIDprototypeDlg::SendStatusInfo(void)
{
	//TODO: �ռ�У��״̬��Ϣ�����͵���������
	//..
	CString strContent, strPhoneEnd, strCenterNum;
	CString count;
	count.Format(_T("%d"), strArray.GetCount());
	CTime now=CTime::GetCurrentTime();
	CString time = now.Format(L"%Y-%m-%d %H:%M:%S");
	struct bus_run bus_run;
	bus_run.bus_run_ID = iniFile.m_strBusRunID;
	bus_run.curr_stu_num = count;
	bus_run.longitude = _T("11.208");
	bus_run.latitude = _T("234.23");
	bus_run.curr_time = time;
	bus_run.curr_speed = _T("89");
	bus_run.curr_station_ID = m_strCurrentStation;
	bus_run.remark = _T("");
	
	strContent = _T("2,");
	strContent += bus_run.bus_run_ID;
	strContent += _T(",");
	strContent += bus_run.curr_stu_num;
	strContent += _T(",");
	strContent += bus_run.longitude;
	strContent += _T(",");
	strContent += bus_run.latitude;
	strContent += _T(",");
	strContent += bus_run.curr_time;
	strContent += _T(",");
	strContent += bus_run.curr_speed;
	strContent += _T(",");
	strContent += bus_run.curr_station_ID;
	strContent += _T(",");
	
	SM_PARAM sm_param;
	USES_CONVERSION;
	CHAR* pch=W2A(strContent);
	strcpy(sm_param.TP_UD, pch);

	strPhoneEnd = _T("86") + iniFile.m_strPhoneNum;
	pch=W2A(strPhoneEnd);
	strcpy(sm_param.TPA, pch);

	strCenterNum = m_strSmsc;
	pch=W2A(strCenterNum);
	strcpy(sm_param.SCA, pch);

	sm_param.TP_PID = 0;
	sm_param.TP_DCS = GSM_8BIT;

#ifdef USE_TRAFFIC
	gsm->PutSendMessage(&sm_param);
#else
	::gsmSendMessage(&sm_param);
#endif
	return 0;
}

void CRFIDprototypeDlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BrowseAndCopy(_T("\\ResidentFlash\\Config\\"), _T("\\SDMMC\\Backup\\Config\\"));
	BrowseAndCopy(_T("\\ResidentFlash\\Sound\\"), _T("\\SDMMC\\Backup\\Sound\\"));
	BrowseAndCopy(_T("\\SDMMC\\Config\\"), _T("\\ResidentFlash\\Config\\"));
	BrowseAndCopy(_T("\\SDMMC\\Sound\\"), _T("\\ResidentFlash\\Sound\\"));
}

//szExistingDir:Դ�ļ���
//szNewDir:Ŀ���ļ���
//ע��:Ŀ���ļ��б���Ҫ����,����ú���������FALSE.
BOOL CRFIDprototypeDlg::BrowseAndCopy(const CString szExistingDir, const CString szNewDir)
{ 
	CString szExistDir;
	CString szAimDir=szNewDir; //����Ŀ���ļ���·��
	CString szFindDir=szExistingDir; //����Դ�ļ���·��

	if(szFindDir.Right(1)!="\\")
	{
		szFindDir+="\\";
		szExistDir=szFindDir;
	}
	szFindDir+="*.*"; //���������ļ�

	WIN32_FIND_DATA fd;
	HANDLE hFind;
	hFind=FindFirstFile(szFindDir,&fd); //Ѱ�ҵ�һ���ļ�
	if(hFind!=INVALID_HANDLE_VALUE)
	{
		do{
			if(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) //�ж��Ƿ��ļ���
			{    
				if(szAimDir.Right(1)!="\\")
				{
					szAimDir+="\\";
				}
				CreateDirectory(szAimDir+fd.cFileName,NULL); //��Ŀ���ļ����д�����Ӧ�����ļ���
				BrowseAndCopy(szExistDir+fd.cFileName,szAimDir+fd.cFileName); //���õݹ�������ļ��µ��ļ�
			}
			else
			{     
				if(szAimDir.Right(1)!="\\")
				{	
					szAimDir+="\\";
				}
		    
				if(CopyFile(szExistDir+fd.cFileName,szAimDir+fd.cFileName,FALSE)==FALSE) //�����ļ���Ŀ���ļ���
				{
				 return FALSE;
				}
			}
		}while(FindNextFile(hFind,&fd)); //�����Ƿ������һ���ļ�
	 }
	 else
	 {
		//Դ�ļ���Ϊ��,����
		return FALSE;
	 } 
	return TRUE;
}