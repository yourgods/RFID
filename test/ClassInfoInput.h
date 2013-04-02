#pragma once

#include "testDoc.h"
#include "common.h"

int ClassInfoCreateDlg(CtestDoc *doc);
int ClassInfoQueryRight(CArray<rowItem, rowItem> &rest);
// CClassInfoInput 对话框

class CClassInfoInput : public CDialog
{
	DECLARE_DYNAMIC(CClassInfoInput)

public:
	CClassInfoInput(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClassInfoInput();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString class_ID;
	CString name;
	CString remark;
	afx_msg void OnBnClickedCancel();
	CString school;
protected:
	virtual void PreInitDialog();
};
