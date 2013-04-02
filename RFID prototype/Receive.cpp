// Receive.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RFID prototype.h"
#include "Receive.h"
#include "SmsTrafic.h"
#include "sms.h"
#include "tools.h"
#include "RFID prototypeDlg.h"

// CReceive �Ի���

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


// CReceive ��Ϣ�������

BOOL CReceive::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_list.InsertColumn(0,_T("���ŷ��ͷ�"),LVCFMT_LEFT,80);
 	m_list.InsertColumn(1,_T("����"),LVCFMT_LEFT,550);

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}