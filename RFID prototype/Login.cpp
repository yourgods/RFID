// Login.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RFID prototype.h"
#include "Login.h"
#include "Sipapi.h"

// CLogin �Ի���

IMPLEMENT_DYNAMIC(CLogin, CDialog)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CLogin::IDD, pParent)
	, m_strName(_T(""))
	, m_strPassword(_T(""))
	, m_strMsg(_T(""))
{

}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strName);
	DDX_Text(pDX, IDC_EDIT3, m_strPassword);
	DDX_Text(pDX, IDC_MSG, m_strMsg);
}


BEGIN_MESSAGE_MAP(CLogin, CDialog)
	ON_BN_CLICKED(IDOK, &CLogin::OnBnClickedOk)
	ON_EN_SETFOCUS(IDC_EDIT2, &CLogin::OnEnSetfocusEdit2)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CLogin::OnEnKillfocusEdit2)
	ON_EN_SETFOCUS(IDC_EDIT3, &CLogin::OnEnSetfocusEdit3)
	ON_EN_KILLFOCUS(IDC_EDIT3, &CLogin::OnEnKillfocusEdit3)
END_MESSAGE_MAP()


// CLogin ��Ϣ�������

void CLogin::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemText(IDC_EDIT2, m_strName);
	GetDlgItemText(IDC_EDIT3, m_strPassword);
	//AfxMessageBox(_T("Name: ") + m_strName + _T("; Password: ") + m_strPassword);
	if(CheckPassword())
		OnOK();
	else{
		m_strMsg = _T("�û������������!");
		SetDlgItemText(IDC_MSG, m_strMsg);
	}
}

BOOL CLogin::CheckPassword(void)
{
	//TODO: �ڴ�����ж������Ƿ���ȷ�ĳ������
	GetDlgItemText(IDC_EDIT2, m_strName);
	GetDlgItemText(IDC_EDIT3, m_strPassword);

	if(m_strName.GetLength() == 0)
		return FALSE;

	if(m_strPassword.GetLength() == 0)
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
