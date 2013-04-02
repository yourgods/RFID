#pragma once

#include "config.h"
// CSetting 对话框

class CSetting : public CDialog
{
	DECLARE_DYNAMIC(CSetting)

public:
	CSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetting();

// 对话框数据
	enum { IDD = IDD_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPort;
	CString m_strRate;
	CString m_strSmsc;
};
