// mdiView.h : CmdiView ��Ľӿ�
//


#pragma once


class CmdiView : public CListView
{
protected: // �������л�����
	CmdiView();
	DECLARE_DYNCREATE(CmdiView)

// ����
public:
	CmdiDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CmdiView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

#ifndef _DEBUG  // mdiView.cpp �еĵ��԰汾
inline CmdiDoc* CmdiView::GetDocument() const
   { return reinterpret_cast<CmdiDoc*>(m_pDocument); }
#endif

