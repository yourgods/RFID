#pragma once

#include "common.h"
#include "database.h"
int DriverInfoCreateDlg(CtestDoc *doc);
int DriverInfoQueryRight(CArray<rowItem, rowItem> &rest);
// CDriverInfoInput �Ի���

class CDriverInfoInput : public CDialog
{
	DECLARE_DYNAMIC(CDriverInfoInput)

public:
	CDriverInfoInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDriverInfoInput();

// �Ի�������
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strDriverID;
	CString m_strName;
	CString m_strSex;
	CString m_strAge;
	CString m_strMobile;
	CString m_strHome;
	CString m_strAddress;
	CString m_strRemark;
	afx_msg void OnBnClickedOk();
};
