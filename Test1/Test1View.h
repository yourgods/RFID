// Test1View.h : CTest1View ��Ľӿ�
//


#pragma once


class CTest1View : public CView
{
protected: // �������л�����
	CTest1View();
	DECLARE_DYNCREATE(CTest1View)

// ����
public:
	CTest1Doc* GetDocument() const;

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
	virtual ~CTest1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // Test1View.cpp �еĵ��԰汾
inline CTest1Doc* CTest1View::GetDocument() const
   { return reinterpret_cast<CTest1Doc*>(m_pDocument); }
#endif

