// Setting.cpp : 实现文件
//

#include "stdafx.h"
#include "RFID prototype.h"
#include "Setting.h"


// CSetting 对话框

IMPLEMENT_DYNAMIC(CSetting, CDialog)

CSetting::CSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CSetting::IDD, pParent)
	, m_strPort(_T(""))
	, m_strRate(_T(""))
	, m_strSmsc(_T(""))
{

}

CSetting::~CSetting()
{
}

void CSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COM, m_strPort);
	DDX_Text(pDX, IDC_RATE, m_strRate);
	DDX_Text(pDX, IDC_CENTER, m_strSmsc);
}


BEGIN_MESSAGE_MAP(CSetting, CDialog)
END_MESSAGE_MAP()


// CSetting 消息处理程序
