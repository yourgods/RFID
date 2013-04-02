#pragma once

#include "config.h"
// CLogin 对话框

class CLogin : public CDialog
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogin();

// 对话框数据
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	CString m_strPassword;
	afx_msg void OnBnClickedOk();
	CString m_strMsg;
	BOOL CheckPassword(void);
	afx_msg void OnEnSetfocusEdit2();
	afx_msg void OnEnKillfocusEdit2();
	afx_msg void OnEnSetfocusEdit3();
	afx_msg void OnEnKillfocusEdit3();
};
