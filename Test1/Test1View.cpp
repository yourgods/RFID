// Test1View.cpp : CTest1View ���ʵ��
//

#include "stdafx.h"
#include "Test1.h"

#include "Test1Doc.h"
#include "Test1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTest1View

IMPLEMENT_DYNCREATE(CTest1View, CView)

BEGIN_MESSAGE_MAP(CTest1View, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CTest1View ����/����

CTest1View::CTest1View()
{
	// TODO: �ڴ˴���ӹ������

}

CTest1View::~CTest1View()
{
}

BOOL CTest1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CTest1View ����

void CTest1View::OnDraw(CDC* /*pDC*/)
{
	CTest1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CTest1View ��ӡ

BOOL CTest1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CTest1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CTest1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CTest1View ���

#ifdef _DEBUG
void CTest1View::AssertValid() const
{
	CView::AssertValid();
}

void CTest1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTest1Doc* CTest1View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTest1Doc)));
	return (CTest1Doc*)m_pDocument;
}
#endif //_DEBUG


// CTest1View ��Ϣ�������
