#pragma once

#include "config.h"
#include "Resource.h"
// CLogin �Ի���

class CLogin : public CDialog
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogin();

// �Ի�������
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTeacherName;
	CString m_strTeacherID;
	CString m_strDriverName;
	CString m_strDriverID;
	afx_msg void OnBnClickedOk();
	CString m_strMsg;
	afx_msg void OnEnSetfocusEdit2();
	afx_msg void OnEnKillfocusEdit2();
	afx_msg void OnEnSetfocusEdit3();
	afx_msg void OnEnKillfocusEdit3();
	BOOL CheckPassword(void);
	HANDLE m_hKillThreadEvent;		// ֪ͨ���̹߳رյ��¼�
	HANDLE m_hThreadKilledEvent;	// ���߳�����رյ��¼�
	static UINT RFID_Thread(LPVOID lpParam);	// ��ȡ˾���ͽ�ʦ��Ϣ
	afx_msg void OnBnClickedCancel();
};
