// serverView.cpp : CserverView 类的实现
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CserverView 构造/析构

CserverView::CserverView()
{
	// TODO: 在此处添加构造代码

}

CserverView::~CserverView()
{
}

BOOL CserverView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CserverView 绘制

void CserverView::OnDraw(CDC* /*pDC*/)
{
	CserverDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CserverView 打印

BOOL CserverView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CserverView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CserverView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CserverView 诊断

#ifdef _DEBUG
void CserverView::AssertValid() const
{
	CView::AssertValid();
}

void CserverView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CserverDoc* CserverView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CserverDoc)));
	return (CserverDoc*)m_pDocument;
}
#endif //_DEBUG


// CserverView 消息处理程序
