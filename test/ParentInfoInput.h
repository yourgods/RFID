#pragma once

#include "common.h"
#include "database.h"
int ParentInfoCreateDlg(CtestDoc *doc);
int ParentInfoQueryRight(CArray<rowItem, rowItem> &rest);

// CParentInfoInput 对话框

class CParentInfoInput : public CDialog
{
	DECLARE_DYNAMIC(CParentInfoInput)

public:
	CParentInfoInput(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CParentInfoInput();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strParentID;
	CString m_strName;
	CString m_strSex;
	CString m_strMobile;
	CString m_strHome;
	CString m_strAddress;
	CString m_strRemark;
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedCancel2();
};
