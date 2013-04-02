// serverView.cpp : CserverView ���ʵ��
//

#include "stdafx.h"
#include "server.h"

#include "serverDoc.h"
#include "serverView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CserverView

IMPLEMENT_DYNCREATE(CserverView, CView)

BEGIN_MESSAGE_MAP(CserverView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CserverView ����/����

CserverView::CserverView()
{
	// TODO: �ڴ˴���ӹ������

}

CserverView::~CserverView()
{
}

BOOL CserverView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CserverView ����

void CserverView::OnDraw(CDC* /*pDC*/)
{
	CserverDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CserverView ��ӡ

BOOL CserverView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CserverView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CserverView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CserverView ���

#ifdef _DEBUG
void CserverView::AssertValid() const
{
	CView::AssertValid();
}

void CserverView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CserverDoc* CserverView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CserverDoc)));
	return (CserverDoc*)m_pDocument;
}
#endif //_DEBUG


// CserverView ��Ϣ�������
