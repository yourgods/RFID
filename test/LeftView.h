#pragma once

#include "testDoc.h"
// CLeftView 视图

class CLeftView : public CTreeView
{
	DECLARE_DYNCREATE(CLeftView)

protected:
	CLeftView();           // 动态创建所使用的受保护的构造函数
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


