// SettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "test.h"
#include "SettingDlg.h"
#include "config.h"


// CSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CSettingDlg, CDialog)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDlg::IDD, pParent)
{
	m_strPort = _T("");
	m_strRate = _T("");
	m_strSmsc = _T("");
}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COM, m_strPort);
	DDX_Text(pDX, IDC_RATE, m_strRate);
	DDX_Text(pDX, IDC_CENTER, m_strSmsc);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSettingDlg ��Ϣ�������

void CSettingDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnOK();
}

BOOL CSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetDlgItemText(IDC_COM, m_strPort);
	SetDlgItemText(IDC_RATE, m_strRate);
	SetDlgItemText(IDC_CENTER, m_strSmsc);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
