#pragma once

#include "common.h"
#include "database.h"

int BusInfoCreateDlg(CtestDoc *doc);
int BusInfoQueryRight(CArray<rowItem, rowItem> &rest);
// CBusInfo �Ի���

class CBusInfoInput : public CDialog
{
	DECLARE_DYNAMIC(CBusInfoInput)

public:
	CBusInfoInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBusInfoInput();

// �Ի�������
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strType;
	CString m_strBusno;
	CString m_strCap;
	CString m_strRemark;
	afx_msg void OnEnSetfocusEdit5();
	//CMonthCalCtrl *ctlCalendar;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_strBuyTime;
	CString m_strCP;
	afx_msg void OnEnKillfocusEdit5();
};
