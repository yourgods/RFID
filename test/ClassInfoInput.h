#pragma once

#include "testDoc.h"
#include "common.h"

int ClassInfoCreateDlg(CtestDoc *doc);
int ClassInfoQueryRight(CArray<rowItem, rowItem> &rest);
// CClassInfoInput �Ի���

class CClassInfoInput : public CDialog
{
	DECLARE_DYNAMIC(CClassInfoInput)

public:
	CClassInfoInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClassInfoInput();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString class_ID;
	CString name;
	CString remark;
	int school_index;
	CArray<rowItem, rowItem> m_aSchool;
	int class_index;
	CArray<rowItem, rowItem> m_aClass;
	afx_msg void OnBnClickedCancel();
	CString school;
protected:
	virtual void PreInitDialog();
public:
	afx_msg void OnCbnSelchangeName();
	CString m_strSchoolNo;
	CString m_strSchoolManager;
	CString m_strSchoolMobile;
	CString m_strSchoolFix;
	CString m_strSchoolRemark;
	afx_msg void OnCbnSelchangeSchool();
};
