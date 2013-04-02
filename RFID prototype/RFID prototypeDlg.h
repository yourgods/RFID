// RFID prototypeDlg.h : 头文件
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

// CRFIDprototypeDlg 对话框
class CRFIDprototypeDlg : public CDialog
{
// 构造
public:
	CRFIDprototypeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RFIDPROTOTYPE_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	// 返回TRUE，说明列表中已经存在该信息，说明是下车；如果FALSE说明是上车信息
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
