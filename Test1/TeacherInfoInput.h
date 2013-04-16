#pragma once

#include "Test1Doc.h"
#include "common.h"
int TeacherInfoCreateDlg(CTest1Doc *doc);
int TeacherInfoQueryRight(CArray<rowItem, rowItem> &rest);

// CTeacherInfoInput 对话框

class CTeacherInfoInput : public CDialog
{
	DECLARE_DYNAMIC(CTeacherInfoInput)

public:
	CTeacherInfoInput(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTeacherInfoInput();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTeacherID;
	CString m_strName;
	CString m_strSex;
	CString m_strAge;
	CString m_strMobile;
	CString m_strHome;
	CString m_strAddress;
	CString m_strRemark;
	afx_msg void OnBnClickedOk();
};
