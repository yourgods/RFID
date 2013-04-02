// testDoc.h : CtestDoc 类的接口
//


#pragma once

#include "SmsTraffic.h"

UINT Recv_Thread(LPVOID pParam);

class CtestDoc : public CDocument
{
protected: // 仅从序列化创建
	CtestDoc();
	DECLARE_DYNCREATE(CtestDoc)

// 属性
public:
	CString m_currentInfo;
// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CtestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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
	HANDLE m_hKillThreadEvent;		// 通知子线程关闭的事件
	HANDLE m_hThreadKilledEvent;	// 子线程宣告关闭的事件
	int SetCurrentInfo(CString currentInfo);
};


