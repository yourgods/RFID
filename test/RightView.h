#pragma once

#include "testDoc.h"
// CRightView 视图

class CRightView : public CListView
{
	DECLARE_DYNCREATE(CRightView)

protected:
	CRightView();           // 动态创建所使用的受保护的构造函数
	virtual ~CRightView();

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
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	virtual CtestDoc* GetDocument();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


