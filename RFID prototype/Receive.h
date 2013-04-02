#pragma once
#include "afxcmn.h"
#include "RFID prototypeDlg.h"
#include "SmsTrafic.h"
#include "config.h"

// CReceive 对话框

class CReceive : public CDialog
{
	DECLARE_DYNAMIC(CReceive)

public:
	CReceive(CWnd* pParent = NULL);   // 标准构造函数
#ifdef USE_TRAFFIC
	CReceive(void *pVoid, CWnd* pParent = NULL);
#endif
	virtual ~CReceive();
	BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_RECEIVE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
#ifdef USE_TRAFFIC
	CSmsTraffic *gsm;
#endif
	afx_msg void OnBnClickedOk();
	BOOL Show();
};
