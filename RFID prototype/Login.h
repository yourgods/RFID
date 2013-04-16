#pragma once

#include "config.h"
#include "Resource.h"
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
	CString m_strTeacherName;
	CString m_strTeacherID;
	CString m_strDriverName;
	CString m_strDriverID;
	afx_msg void OnBnClickedOk();
	CString m_strMsg;
	afx_msg void OnEnSetfocusEdit2();
	afx_msg void OnEnKillfocusEdit2();
	afx_msg void OnEnSetfocusEdit3();
	afx_msg void OnEnKillfocusEdit3();
	BOOL CheckPassword(void);
	HANDLE m_hKillThreadEvent;		// 通知子线程关闭的事件
	HANDLE m_hThreadKilledEvent;	// 子线程宣告关闭的事件
	static UINT RFID_Thread(LPVOID lpParam);	// 读取司机和教师信息
	afx_msg void OnBnClickedCancel();
};
