// mdiView.cpp : CmdiView 类的实现
//

#include "stdafx.h"
#include "mdi.h"

#include "mdiDoc.h"
#include "mdiView.h"
#include "database.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmdiView

IMPLEMENT_DYNCREATE(CmdiView, CListView)

BEGIN_MESSAGE_MAP(CmdiView, CListView)
	ON_WM_STYLECHANGED()
END_MESSAGE_MAP()

// CmdiView 构造/析构

CmdiView::CmdiView()
{
	// TODO: 在此处添加构造代码

}

CmdiView::~CmdiView()
{
}

BOOL CmdiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CListView::PreCreateWindow(cs);
}

void CmdiView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: 调用 GetListCtrl() 直接访问 ListView 的列表控件，
	//  从而可以用项填充 ListView。
}


// CmdiView 诊断

#ifdef _DEBUG
void CmdiView::AssertValid() const
{
	CListView::AssertValid();
}

void CmdiView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CmdiDoc* CmdiView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmdiDoc)));
	return (CmdiDoc*)m_pDocument;
}
#endif //_DEBUG


// CmdiView 消息处理程序
void CmdiView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: 添加代码以响应用户对窗口视图样式的更改	
	CListView::OnStyleChanged(nStyleType,lpStyleStruct);	
}

void CmdiView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: 在此添加专用代码和/或调用基类
	CString temp = GetDocument()->m_currentInfo;
	int index = 0;
	for(index = 0; index<sizeof(Table)/sizeof(Table[0]); index++){
		if(temp.Find(Table[index].chineseName) != -1)
			break;
	}
	GetListCtrl().SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	for(int i = 0; i<Table[index].itemKeyCount + Table[index].itemOthersCount; i++){
		GetListCtrl().InsertColumn(i,Table[index].fieldValue[i].chineseName,LVCFMT_LEFT,80);
	}
}
