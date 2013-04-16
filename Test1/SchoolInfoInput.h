#pragma once

#include "Test1Doc.h"
#include "common.h"

int SchoolInfoCreateDlg(CTest1Doc *doc);
int SchooInfoQueryRight(CArray<rowItem, rowItem> &);
// CSchoolInfoInput �Ի���

class CSchoolInfoInput : public CDialog
{
	DECLARE_DYNAMIC(CSchoolInfoInput)

public:
	CSchoolInfoInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSchoolInfoInput();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk2();
	CString school_ID;
	CString name;
	CString manager;
	CString mobi_phone;
	CString fix_phone;
	CString remark;
	CArray<rowItem, rowItem> m_aSchool;
	int school_index;
	afx_msg void OnBnClickedCancel2();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreInitDialog();
public:
	afx_msg void OnCbnSelchangeName();
	void UpdateInfo(void);
};
