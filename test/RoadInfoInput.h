#pragma once

#include "common.h"
#include "database.h"
int RoadInfoCreateDlg(CtestDoc *doc);
int RoadInfoQueryRight(CArray<rowItem, rowItem> &rest);
// CRoadInfoInput 对话框

class CRoadInfoInput : public CDialog
{
	DECLARE_DYNAMIC(CRoadInfoInput)

public:
	CRoadInfoInput(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRoadInfoInput();

// 对话框数据
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strRoadID;
	CString m_strName;
	CString m_strStart;
	CString m_strEnd;
	CString m_strRemark;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
