// testDoc.cpp : CtestDoc ���ʵ��
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


// CtestDoc ����/����

CtestDoc::CtestDoc()
: gsm(NULL)
{
	// TODO: �ڴ����һ���Թ������
	m_strCurrentInfo = _T("ѧУ��Ϣ");
	int err = 0;
	if(err = OpenDB()){
		CString temp;
		temp.Format(_T("%d"), err);
		MessageBox(NULL, _T("�����ݿ����,�����:") + temp, _T("�����ݿ�"), MB_OK);
	}else{
		//::MessageBox(NULL, _T("�����ݿ�ɹ�"), _T("�����ݿ�"), MB_OK);	
	}

	//���ô��ڲ���Ĭ��ֵ
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

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CtestDoc ���л�

void CtestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CtestDoc ���

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
	// TODO: �ڴ���������������
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
		strError.Format(L"�޷��򿪶˿�%s! ����������?", m_strPort);
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
		strError.Format(L"�˿�%s��û�з���MODEM!", m_strPort);
		AfxMessageBox(strError, MB_OK);
		return;
	}

	if (m_strSmsc.IsEmpty())
	{
		AfxMessageBox(L"������SMSC!");
		 
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

	//AfxMessageBox(_T("����GSM�������ɹ�!"));

	gsm = new CSmsTraffic;
	//start receive thread
	AfxBeginThread(Recv_Thread, this);
}

void CtestDoc::On32772()
{
	// TODO: �ڴ���������������
	SetEvent(m_hKillThreadEvent);			// �����ر����̵߳��ź�
	WaitForSingleObject(m_hThreadKilledEvent, INFINITE);	// �ȴ����̹߳ر�
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

	// �ø��߳̽�����־
	SetEvent(pDoc->m_hThreadKilledEvent);
	return nState;
}

int CtestDoc::SetCurrentInfo(CString currentInfo)
{
	m_strCurrentInfo = currentInfo;
	return 0;
}
