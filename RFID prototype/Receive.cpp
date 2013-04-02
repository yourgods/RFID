// Receive.cpp : 实现文件
//

#include "stdafx.h"
#include "RFID prototype.h"
#include "Receive.h"
#include "SmsTrafic.h"
#include "sms.h"
#include "tools.h"
#include "RFID prototypeDlg.h"

// CReceive 对话框

IMPLEMENT_DYNAMIC(CReceive, CDialog)

CReceive::CReceive(CWnd* pParent /*=NULL*/)
	: CDialog(CReceive::IDD, pParent)
{
}
#ifdef USE_TRAFFIC
CReceive::CReceive(void *pVoid, CWnd* pParent)
	:CDialog(CReceive::IDD, pParent)
{
	CRFIDprototypeDlg *pp = (CRFIDprototypeDlg *)pVoid;
	gsm = pp->gsm;
}
#endif

CReceive::~CReceive()
{
}

void CReceive::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_list);
}


BEGIN_MESSAGE_MAP(CReceive, CDialog)
	ON_BN_CLICKED(IDOK, &CReceive::OnBnClickedOk)
END_MESSAGE_MAP()


// CReceive 消息处理程序

BOOL CReceive::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_list.InsertColumn(0,_T("短信发送方"),LVCFMT_LEFT,80);
 	m_list.InsertColumn(1,_T("内容"),LVCFMT_LEFT,550);

	Show();
	return TRUE;
}

BOOL CReceive::Show(){
	SM_PARAM tmp;
	int i = 0;
	while(gsm->GetRecvMessage(&tmp)){
		CString strFrom(tmp.TPA);
		CString strMsg(tmp.TP_UD);
		m_list.InsertItem(i,(LPCTSTR)strFrom);
		m_list.SetItemText(i, 1, (LPCTSTR)strMsg);
		i++;
	}
	return TRUE;
}

void CReceive::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}