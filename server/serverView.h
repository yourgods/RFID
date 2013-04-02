// serverView.h : CserverView ��Ľӿ�
//


#pragma once


class CserverView : public CView
{
protected: // �������л�����
	CserverView();
	DECLARE_DYNCREATE(CserverView)

// ����
public:
	CserverDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CserverView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // serverView.cpp �еĵ��԰汾
inline CserverDoc* CserverView::GetDocument() const
   { return reinterpret_cast<CserverDoc*>(m_pDocument); }
#endif

