#pragma once

#include "common.h"
#include "database.h"
int RoadInfoCreateDlg(CtestDoc *doc);
int RoadInfoQueryRight(CArray<rowItem, rowItem> &rest);
// CRoadInfoInput �Ի���

class CRoadInfoInput : public CDialog
{
	DECLARE_DYNAMIC(CRoadInfoInput)

public:
	CRoadInfoInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRoadInfoInput();

// �Ի�������
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
