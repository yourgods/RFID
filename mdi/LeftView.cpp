// LeftView.cpp : CLeftView 类的实现
//

#include "stdafx.h"
#include "mdi.h"
#include "common.h"
#include "mdiDoc.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(NM_CLICK, &CLeftView::OnNMClick)
END_MESSAGE_MAP()


// CLeftView 构造/析构

CLeftView::CLeftView()
{
	// TODO: 在此处添加构造代码
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式

	return CTreeView::PreCreateWindow(cs);
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: 调用 GetTreeCtrl() 直接访问 TreeView 的树控件，
	//  从而可以用项填充 TreeView。
	HTREEITEM hItem;
	hItem = GetTreeCtrl().InsertItem(_T("学校信息"), TVI_ROOT);
	hItem = GetTreeCtrl().InsertItem(_T("班级信息"), TVI_ROOT, hItem);
	hItem = GetTreeCtrl().InsertItem(_T("学生信息"), TVI_ROOT, hItem);
	hItem = GetTreeCtrl().InsertItem(_T("教师信息"), TVI_ROOT, hItem);
	hItem = GetTreeCtrl().InsertItem(_T("家长信息"), TVI_ROOT, hItem);
	hItem = GetTreeCtrl().InsertItem(_T("校车信息"), TVI_ROOT, hItem);
	hItem = GetTreeCtrl().InsertItem(_T("司机信息"), TVI_ROOT, hItem);
	hItem = GetTreeCtrl().InsertItem(_T("校车线路信息"), TVI_ROOT, hItem);
	hItem = GetTreeCtrl().InsertItem(_T("站点信息"), TVI_ROOT, hItem);
}


// CLeftView 诊断

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CmdiDoc* CLeftView::GetDocument() // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmdiDoc)));
	return (CmdiDoc*)m_pDocument;
}
#endif //_DEBUG


// CLeftView 消息处理程序

void CLeftView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CString temp = GetTreeCtrl().GetItemText(hItem);
	//增加“添加****”子条目
	if(GetTreeCtrl().GetParentItem(hItem) == NULL && GetTreeCtrl().ItemHasChildren(hItem) == 0){
		CString subItemText = _T("添加");
		subItemText.Append(temp);
		GetTreeCtrl().InsertItem(subItemText, hItem);
	}
	GetDocument()->m_currentInfo = temp;
	GetDocument()->UpdateAllViews(this);
	//if(!GetDocument()->QueryDB(temp)){
	//	MessageBox(_T("查询数据库错误:") + temp);
	//}else{
	//	MessageBox(_T("查询数据库成功:") + temp);	
	//}
	CTreeView::OnLButtonDblClk(nFlags, point);
}

void CLeftView::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
		//查询数据库并显示查询到的信息
	//Sleep(100);
	//HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	//CString temp = GetTreeCtrl().GetItemText(hItem);
	//if(!QueryDB(temp)){
	//	MessageBox(_T("查询数据库错误:") + temp);
	//}else{
	//	MessageBox(_T("查询数据库成功:") + temp);	
	//}
	*pResult = 0;
}
