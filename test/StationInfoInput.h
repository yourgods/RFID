#pragma once

#include "common.h"
#include "database.h"
int StationInfoCreateDlg(CtestDoc *doc);
int StationInfoQueryRight(CArray<rowItem, rowItem> &rest);
// CStationInfoInput 对话框

class CStationInfoInput : public CDialog
{
	DECLARE_DYNAMIC(CStationInfoInput)

public:
	CStationInfoInput(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStationInfoInput();

// 对话框数据
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strStationID;
	CString m_strName;
	CString m_strRoadID;
	CString m_strRemark;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
