// mdiView.h : CmdiView 类的接口
//


#pragma once


class CmdiView : public CListView
{
protected: // 仅从序列化创建
	CmdiView();
	DECLARE_DYNCREATE(CmdiView)

// 属性
public:
	CmdiDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CmdiView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

#ifndef _DEBUG  // mdiView.cpp 中的调试版本
inline CmdiDoc* CmdiView::GetDocument() const
   { return reinterpret_cast<CmdiDoc*>(m_pDocument); }
#endif

