// testDoc.cpp : CtestDoc 类的实现
//

#include "stdafx.h"
#include "test.h"
#include "database.h"
#include "common.h"
#include "testDoc.h"
#include "Comm.h"
#include "sms.h"
#include "SettingDlg.h"
#include "tools.h"
#include "config.h"
#include "message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CtestDoc

IMPLEMENT_DYNCREATE(CtestDoc, CDocument)

BEGIN_MESSAGE_MAP(CtestDoc, CDocument)
	ON_COMMAND(ID_32771, &CtestDoc::On32771)
	ON_COMMAND(ID_32772, &CtestDoc::On32772)
END_MESSAGE_MAP()


// CtestDoc 构造/析构

CtestDoc::CtestDoc()
: gsm(NULL)
{
	// TODO: 在此添加一次性构造代码
	m_strCurrentInfo = _T("学校信息");
	int err = 0;
	if(err = OpenDB()){
		CString temp;
		temp.Format(_T("%d"), err);
		MessageBox(NULL, _T("打开数据库错误,错误号:") + temp, _T("打开数据库"), MB_OK);
	}else{
		//::MessageBox(NULL, _T("打开数据库成功"), _T("打开数据库"), MB_OK);	
	}

	//设置串口参数默认值
	m_strPort = GSM_COM;
	m_strRate = _T("9600");
	m_strSmsc = SMSC;
	m_hKillThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hThreadKilledEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CtestDoc::~CtestDoc()
{
	CloseDB();
}

BOOL CtestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CtestDoc 序列化

void CtestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CtestDoc 诊断

#ifdef _DEBUG
void CtestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CtestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

void CtestDoc::On32771()
{
	// TODO: 在此添加命令处理程序代码
	CSettingDlg dlg;
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
		return;
	}

	int nRate;
	nRate = _ttoi(m_strRate);

	char *port = UnicodeToAnsi(m_strPort.GetBuffer(m_strPort.GetLength()));

	while (!::OpenComm(port, nRate))
	{
		delete[] port;
		CString strError;
		strError.Format(L"无法打开端口%s! 现在设置吗?", m_strPort);
		if (AfxMessageBox(strError, MB_YESNO) == IDNO) return;

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
			return;
		}
		port = UnicodeToAnsi(m_strPort.GetBuffer(m_strPort.GetLength()));
	}

	if (!gsmInit())
	{
		CString strError;
		strError.Format(L"端口%s上没有发现MODEM!", m_strPort);
		AfxMessageBox(strError, MB_OK);
		return;
	}

	if (m_strSmsc.IsEmpty())
	{
		AfxMessageBox(L"请设置SMSC!");
		 
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
 			return;
 		}
	}

	//AfxMessageBox(_T("连接GSM发射器成功!"));

	gsm = new CSmsTraffic;
	//start receive thread
	AfxBeginThread(Recv_Thread, this);
}

void CtestDoc::On32772()
{
	// TODO: 在此添加命令处理程序代码
	SetEvent(m_hKillThreadEvent);			// 发出关闭子线程的信号
	WaitForSingleObject(m_hThreadKilledEvent, INFINITE);	// 等待子线程关闭
	gsmDeinit();
	CloseComm();
}

int char2int(char *msg, int len){
	int i = 0;
	int ret = 0;
	for(i = 0; i<len; i++){
		if(msg[i] >= '0' && msg[i] <= '9'){
			ret *= 10;
			ret += msg[i] - '0';
		}else{
			return ret;
		}
	}
	return ret;
}

UINT Recv_Thread(LPVOID pParam){
	CtestDoc *pDoc = (CtestDoc *)pParam;
	CSmsTraffic *gsm = pDoc->gsm;
	SM_PARAM param;
	int nState = true;
	char *msg = param.TP_UD;
	while(nState){
		if(gsm->GetRecvMessage(&param)){
			char *currPos = strstr(msg, ",");
			int msgId = char2int(msg, currPos - msg);
			msg = currPos + 1;
			struct update update;
			switch(msgId){
			case 1:
				parseBusDepart(msg, &update);
				InsertUniform(&update);
				break;
			case 2:
				parseBusRun(msg, &update);
				Insert(&update);
				break;
			case 3:
				parseBusStudent(msg, &update);
				InsertUniform(&update);
				break;
			default:
				break;
			}
		}else{
			Sleep(1000);
		}
		DWORD dwEvent = WaitForSingleObject(pDoc->m_hKillThreadEvent, 20);
		if (dwEvent == WAIT_OBJECT_0)  nState = false;
	}

	// 置该线程结束标志
	SetEvent(pDoc->m_hThreadKilledEvent);
	return nState;
}

int CtestDoc::SetCurrentInfo(CString currentInfo)
{
	m_strCurrentInfo = currentInfo;
	return 0;
}
