#pragma once

#include "common.h"
#include "database.h"
int StudentInfoCreateDlg(CTest1Doc *doc);
int StudentInfoQueryRight(CArray<rowItem, rowItem> &rest);
// CStudentInfoInput 对话框

class CStudentInfoInput : public CDialog
{
	DECLARE_DYNAMIC(CStudentInfoInput)

public:
	CStudentInfoInput(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStudentInfoInput();

// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strStudentID;
	CString m_strName;
	CString m_strSex;
	CString m_strAge;
	CString m_strnroll;
	CString m_strAddress;
	CString m_strParentID;
	CString m_strTeacher;
	CString m_strRemark;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_strClass;
};
