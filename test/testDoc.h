// testDoc.h : CtestDoc ��Ľӿ�
//


#pragma once

#include "SmsTraffic.h"

UINT Recv_Thread(LPVOID pParam);

class CtestDoc : public CDocument
{
protected: // �������л�����
	CtestDoc();
	DECLARE_DYNCREATE(CtestDoc)

// ����
public:
	CString m_currentInfo;
// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CtestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void On32771();
	afx_msg void On32772();
	CString m_strCurrentInfo;
	CString m_strPort;
	CString m_strRate;
	CString m_strSmsc;
	CSmsTraffic *gsm;
	HANDLE m_hKillThreadEvent;		// ֪ͨ���̹߳رյ��¼�
	HANDLE m_hThreadKilledEvent;	// ���߳�����رյ��¼�
	int SetCurrentInfo(CString currentInfo);
};


