// RFID prototypeDlg.cpp : 实现文件
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

// CRFIDprototypeDlg 对话框
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


// CRFIDprototypeDlg 消息处理程序

BOOL CRFIDprototypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//背景图片
	m_BackScreenBitmap.LoadBitmap(IDB_BKG);

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//设置主界面为全屏并隐藏任务栏
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
	// TODO: 在此添加额外的初始化代码
	// 登陆校车安全系统并获取随车教师和司机编码
	CLogin loginDlg;
	if (loginDlg.DoModal() == IDOK)
	{
#ifdef DEBUG_YFH
		//::MessageBox(NULL, _T("登陆成功"), _T("登陆"), MB_OK);
#endif
		m_strTeacherID = loginDlg.m_strName;
		m_strDriverID = DRIVER_ID;
	}else{
		::MessageBox(NULL, _T("登陆失败,退出系统!"), _T("登陆"), MB_OK);
		OnOK();
	}

	//从ini文件中读取配置,内容包括:车牌号,校车编号,发车次编号,学校电话号码,最大车载人数,路径编号及各站点编号
	iniFile.WriteIni();
	if(iniFile.ReadIni())
	{
		////显示读出的ini文件
		//CString strIni;
		//strIni += _T("车牌号:");
		//strIni += iniFile.m_strCP;
		//strIni += _T("\r\n最大车载人数:");
		//strIni += iniFile.m_strCount;
		//strIni += _T("\r\n学校号码:");
		//strIni += iniFile.m_strPhoneNum;
		//strIni += _T("\r\n校车编号:");
		//strIni += iniFile.m_strBusID;
		//strIni += _T("\r\n路径编号:");
		//strIni += iniFile.m_strRoadID;
		//strIni += _T("\r\n发车次编号:");
		//strIni += iniFile.m_strBusRunID;
		//::MessageBox(NULL, strIni, _T("配置"), MB_OK);
	}else{
		::MessageBox(NULL, _T("无法读出配置文件,将使用默认配置!"), _T("配置"), MB_OK);
	}	

	gsm = new CSmsTraffic();
	if(gsm == NULL){
		::MessageBox(NULL, _T("无法启动gsm设备,请联系设备提供商!"), _T("配置"), MB_OK);
		OnOK();
	}

	//初始化List control
	InitList();

	//读取数据库(doing)
	CheckDataBaseExist();

	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);

	iniFile.WriteIniBusRunID();
	noStation = 0;
	m_strCurrentStation = iniFile.m_cStations.GetAt(noStation);

	//启动RFID模块，进行读卡操作
	//启动接收线程。
	if (!GetExitCodeThread(hReadThread, &dwTStat) ||(dwTStat != STILL_ACTIVE)) 
	{
		hReadThread = CreateThread (NULL, 0,RXINT_Thread, this,0, &dwTStat);
	}
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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
	// TODO: 在此添加控件通知处理程序代码
	CString strContent,strPhoneEnd,strCenterNum,strUpTime,strDownTime;

	CString strTempStart;
	strTempStart = "86";
	int  NumUp = 0,NumDown = 0;
	strCenterNum = SMSC;

	//上车人数
	CString strTemp2;	
	NumUp= strArray.GetCount();
	strTemp2.Format(_T("%d"),NumUp);

	//上下车时间
	strUpTime = m_ListPassenger.GetItemText(0,2);
	strDownTime = m_ListPassenger.GetItemText(NumUp - 1,4);

	//服务器号码
	strPhoneEnd = strTempStart + iniFile.m_strPhoneNum;
	
	//下车人数
	StructPerson stPerson;
	for (int j=0;j<strArray.GetCount();j++)
	{
		stPerson = strArray.GetAt(j);
		if(stPerson.strDownMessage == "是")
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
		::MessageBox(NULL, _T("\\ResidentFlash\\Sound\\Warning.wav"), _T("上下车人数不等!"), MB_OK);
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
	//接收列表初始化
	CRect rect;
	GetWindowRect(&rect);
	
	((CListCtrl *)GetDlgItem(IDC_LIST1))->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	((CListCtrl *)GetDlgItem(IDC_LIST1))->InsertColumn(0,_T("姓名"),LVCFMT_LEFT,80);
 	((CListCtrl *)GetDlgItem(IDC_LIST1))->InsertColumn(1,_T("家长联系方式"),LVCFMT_LEFT,120);
 	((CListCtrl *)GetDlgItem(IDC_LIST1))->InsertColumn(3,_T("上车时间"),LVCFMT_LEFT,170);
 	((CListCtrl *)GetDlgItem(IDC_LIST1))->InsertColumn(4,_T("上车短信"),LVCFMT_LEFT,80);
 	((CListCtrl *)GetDlgItem(IDC_LIST1))->InsertColumn(5,_T("下车时间"),LVCFMT_LEFT,170);
 	((CListCtrl *)GetDlgItem(IDC_LIST1))->InsertColumn(6,_T("下车短信"),LVCFMT_LEFT,80);
 	((CListCtrl *)GetDlgItem(IDC_LIST1))->InsertColumn(7,_T("车牌号"),LVCFMT_LEFT,80);
 	((CListCtrl *)GetDlgItem(IDC_LIST1))->InsertColumn(8,_T("ID"),LVCFMT_LEFT,0);
}

void CRFIDprototypeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	//画站点线
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
	// TODO: 在此添加专用代码和/或调用基类
	//显示隐藏的任务栏
	HWND hTask = ::FindWindow(TEXT("HHTaskBar"), NULL);
	if (hTask)
	{
	::ShowWindow(hTask, SW_SHOW);
	}
	CDialog::OnCancel();
}

void CRFIDprototypeDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	//显示隐藏的任务栏
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

// 返回TRUE，说明列表中已经存在该信息，说明是下车；如果FALSE说明是上车信息
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
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	//////////////////////////////////////////////////////////////////////////
    //测试播放声音
	//PlaySound(L"\\ResidentFlash\\Sound\\close.wav",NULL,SND_SYNC|SND_NODEFAULT);
	//////////////////////////////////////////////////////////////////////////

	//把发车次信息存入数据库中
	struct bus_depart bus_depart;
	bus_depart.bus_run_ID = iniFile.m_strBusRunID;
	bus_depart.bus_ID = iniFile.m_strBusID;
	bus_depart.driver_ID = m_strDriverID;
	bus_depart.teacher_ID = m_strTeacherID;
	bus_depart.road_ID = iniFile.m_strRoadID;
	bus_depart.student_number = iniFile.m_strCount;
	bus_depart.remark = _T("");
	InsertBusDepart(bus_depart);

	//把发车次信息发送到服务器
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

	//	//把校车学生信息存储到数据库中
	//	struct bus_stu bus_stu;
	//	bus_stu.bus_run_ID = iniFile.m_strBusRunID;
	//	bus_stu.student_ID = strtt.strID;
	//	bus_stu.up_station_ID = strtt.strUpStation;
	//	bus_stu.up_time = strtt.strUpTime;
	//	bus_stu.down_station_ID = strtt.strDownStation;
	//	bus_stu.down_time = strtt.strDownTime;
	//	bus_stu.remark = _T("");
	//	InsertBusStu(bus_stu);

	//	if(strtt.strDownMessage.Compare(_T("是")) == 0 && strtt.strUpMessage.Compare(_T("是")) == 0)
	//		continue;

	//	if((!strtt.strDownTime.IsEmpty())&&(strtt.strDownMessage.Compare(_T("否")) == 0))
	//		strtt.strDownMessage = _T("是");
	//	if((!strtt.strUpTime.IsEmpty())&&(strtt.strUpMessage.Compare(_T("否")) == 0))
	//		strtt.strUpMessage = _T("是");	

	//	strArray.SetAt(j, strtt);
	//	//发送校车学生信息到服务器中
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
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox(L"上车短信");
	CString strName,strContent,strendContent,strPhone,strPhoneEnd,strCenterNum,strUpTime;
	strCenterNum = SMSC;
	CString strTempStart;
	strTempStart = "86";
	strendContent = "您的小孩已上车，车牌号为：";
	strendContent += iniFile.m_strCP;
	CString strSendFlag;

	//ergodic each student
	for (int n=0;n<m_ListPassenger.GetItemCount();n++)
	{
		strName = m_ListPassenger.GetItemText(n,0);
		strPhone = m_ListPassenger.GetItemText(n,1);
		strUpTime = m_ListPassenger.GetItemText(n,2);
		strSendFlag = m_ListPassenger.GetItemText(n,3);

		if (strSendFlag=="否")
		{
			//send upstation message to the student's parent
			strContent = "尊敬的";
			strContent+=strName;
			strContent+="家长：";

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
					stPerson.strUpMessage = "是";
					strArray.SetAt(j,stPerson);
					UpdateSchoolBusUp(stPerson);

					//将校车学生信息存储在本地数据库中
					struct bus_stu bus_stu;
					bus_stu.bus_run_ID = iniFile.m_strBusRunID;
					bus_stu.student_ID = stPerson.strID;
					bus_stu.up_station_ID = stPerson.strUpStation;
					bus_stu.up_time = stPerson.strUpTime;
					bus_stu.down_station_ID = stPerson.strDownStation;
					bus_stu.down_time = stPerson.strDownTime;
					bus_stu.remark = _T("");
					InsertBusStu(bus_stu);

					//发送校车学生信息到服务器中
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
	// TODO: 在此添加控件通知处理程序代码
	CString strName,strContent,strendContent,strPhone,strPhoneEnd,strCenterNum,strUpTime,strDownTime;
	strCenterNum = SMSC;
	CString strTempStart;
	strTempStart = "86";
	strendContent = "您的小孩已下车，车牌号为：";
	strendContent+= iniFile.m_strCP;
	CString strSendFlag;
	for (int n=0;n<m_ListPassenger.GetItemCount();n++)
	{
		strName = m_ListPassenger.GetItemText(n,0);
		strPhone = m_ListPassenger.GetItemText(n,1);
		strUpTime = m_ListPassenger.GetItemText(n,2);
		strSendFlag = m_ListPassenger.GetItemText(n,5);
		strDownTime = m_ListPassenger.GetItemText(n,4);

		if (strSendFlag=="否"&&strDownTime.Trim()!="")
		{
			strContent = "尊敬的";
			strContent+=strName;
			strContent+="：";

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
					stPerson.strDownMessage = "是";
					strArray.SetAt(j,stPerson);
					//CTime Downt=CTime::GetCurrentTime();
					//CString strDownTime = Downt.Format(L"%Y-%m-%d %H:%M:%S");
					UpdateSchoolBusDown(stPerson, stPerson.strDownTime);

					//更新数据库中学生上下车的时间站点信息
					struct bus_stu bus_stu;
					bus_stu.bus_run_ID = iniFile.m_strBusRunID;
					bus_stu.student_ID = stPerson.strID;
					bus_stu.up_station_ID = stPerson.strUpStation;
					bus_stu.up_time = stPerson.strUpTime;
					bus_stu.down_station_ID = stPerson.strDownStation;
					bus_stu.down_time = stPerson.strDownTime;
					bus_stu.remark = _T("");
					InsertBusStu(bus_stu);

					//发送校车学生信息到服务器中
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
	// TODO: 在此添加控件通知处理程序代码
#ifdef USE_TRAFFIC
	CReceive dlg(this);
#else
	CReceive dlg;
#endif
	dlg.DoModal();
}

void CRFIDprototypeDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	noStation++;
	m_strCurrentStation = iniFile.m_cStations.GetAt(noStation);
	//播放站点声音
	CString strSoundPath;
	strSoundPath.Format(_T("\\ResidentFlash\\Sound\\Station%d.wav"), noStation);
	//PlaySound(strSoundPath,NULL,SND_SYNC|SND_NODEFAULT);
	::MessageBox(NULL, _T("播放站点声音:") + strSoundPath, _T("Station"), MB_OK);
	Invalidate();
}

void CRFIDprototypeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent){
		case TIMER:
			 //AfxMessageBox(_T("定时器1"));
			SendStatusInfo();
			break;
		default:
			break;
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CRFIDprototypeDlg::SendStatusInfo(void)
{
	//TODO: 收集校车状态信息并发送到服务器端
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
	// TODO: 在此添加控件通知处理程序代码
	BrowseAndCopy(_T("\\ResidentFlash\\Config\\"), _T("\\SDMMC\\Backup\\Config\\"));
	BrowseAndCopy(_T("\\ResidentFlash\\Sound\\"), _T("\\SDMMC\\Backup\\Sound\\"));
	BrowseAndCopy(_T("\\SDMMC\\Config\\"), _T("\\ResidentFlash\\Config\\"));
	BrowseAndCopy(_T("\\SDMMC\\Sound\\"), _T("\\ResidentFlash\\Sound\\"));
}

//szExistingDir:源文件夹
//szNewDir:目标文件夹
//注意:目标文件夹必须要存在,否则该函数将返回FALSE.
BOOL CRFIDprototypeDlg::BrowseAndCopy(const CString szExistingDir, const CString szNewDir)
{ 
	CString szExistDir;
	CString szAimDir=szNewDir; //保存目标文件夹路径
	CString szFindDir=szExistingDir; //保存源文件夹路径

	if(szFindDir.Right(1)!="\\")
	{
		szFindDir+="\\";
		szExistDir=szFindDir;
	}
	szFindDir+="*.*"; //搜索所有文件

	WIN32_FIND_DATA fd;
	HANDLE hFind;
	hFind=FindFirstFile(szFindDir,&fd); //寻找第一个文件
	if(hFind!=INVALID_HANDLE_VALUE)
	{
		do{
			if(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) //判断是否文件夹
			{    
				if(szAimDir.Right(1)!="\\")
				{
					szAimDir+="\\";
				}
				CreateDirectory(szAimDir+fd.cFileName,NULL); //在目标文件夹中创建相应的子文件夹
				BrowseAndCopy(szExistDir+fd.cFileName,szAimDir+fd.cFileName); //采用递归查找子文件下的文件
			}
			else
			{     
				if(szAimDir.Right(1)!="\\")
				{	
					szAimDir+="\\";
				}
		    
				if(CopyFile(szExistDir+fd.cFileName,szAimDir+fd.cFileName,FALSE)==FALSE) //拷贝文件到目标文件夹
				{
				 return FALSE;
				}
			}
		}while(FindNextFile(hFind,&fd)); //查找是否存在下一个文件
	 }
	 else
	 {
		//源文件夹为空,返回
		return FALSE;
	 } 
	return TRUE;
}