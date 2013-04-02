// RFID prototypeDlg.h : ͷ�ļ�
//

#pragma once
#include "inifile.h"
#include "resource.h"
#include "afxcmn.h"
#include "SmsTrafic.h"
#include "config.h"

struct StructPerson
{
	CString strName;
	CString strID;
	CString strPhoneNum;
	CString strUpStation;
	CString strUpTime;
	CString strUpMessage;//0,no send;1 have sended
	CString strDownStation;
	CString strDownTime;
	CString strDownMessage;
};

extern char send_data[1024];
extern CIniFile iniFile;
extern CArray<StructPerson,StructPerson> strArray;

// CRFIDprototypeDlg �Ի���
class CRFIDprototypeDlg : public CDialog
{
// ����
public:
	CRFIDprototypeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RFIDPROTOTYPE_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton5();
	void InitList(void);
	afx_msg void OnPaint();
	CBitmap m_BackScreenBitmap;
	void FlushPoint(CPaintDC *dc, int index, int start, int length, int y);
protected:
	virtual void OnCancel();
	virtual void OnOK();
public:
	void UpdateList(void);
	CListCtrl m_ListPassenger;
	// ����TRUE��˵���б����Ѿ����ڸ���Ϣ��˵�����³������FALSE˵�����ϳ���Ϣ
	BOOL CheckDataExist(CString strName, CString strPhone);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	CString m_strDriverID;
	CString m_strTeacherID;
	CString m_strCurrentStation;
	CString m_strSmsc;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL SendStatusInfo(void);
	afx_msg void OnBnClickedButton7();
	BOOL BrowseAndCopy(CString, CString);
#ifdef USE_TRAFFIC
	CSmsTraffic *gsm;
#endif
};
