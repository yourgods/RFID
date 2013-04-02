#pragma once

#include "testDoc.h"
// CLeftView ��ͼ

class CLeftView : public CTreeView
{
	DECLARE_DYNCREATE(CLeftView)

protected:
	CLeftView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CLeftView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual CtestDoc* GetDocument();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


