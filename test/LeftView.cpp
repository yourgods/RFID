// LeftView.cpp : 实现文件
//

#include "stdafx.h"
#include "test.h"
#include "LeftView.h"
#include "database.h"
#include "testDoc.h"
#include "SchoolInfoInput.h"

// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

CLeftView::CLeftView()
{
}

CLeftView::~CLeftView()
{
}

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CLeftView 诊断

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif

CtestDoc* CLeftView::GetDocument() // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CtestDoc)));
	return (CtestDoc*)m_pDocument;
}
#endif //_DEBUG


// CLeftView 消息处理程序

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	long lStyleOld = GetTreeCtrl().GetStyle();
	lStyleOld |=TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_DISABLEDRAGDROP|TVS_SHOWSELALWAYS;
	SetWindowLong(GetTreeCtrl().m_hWnd, GWL_STYLE, lStyleOld);
	GetTreeCtrl().SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);

	// TODO: 调用 GetTreeCtrl() 直接访问 TreeView 的树控件，
	//  从而可以用项填充 TreeView。
	HTREEITEM hItem;
	for(int i =0 ; i<12; i++){
		hItem = GetTreeCtrl().InsertItem(Table[i].chineseName, TVI_ROOT);
		if(Table[i].chineseName.Find(_T("关系")) == -1)
			GetTreeCtrl().InsertItem(_T("添加") + Table[i].chineseName, hItem);
	}
}

void CLeftView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CString temp = GetTreeCtrl().GetItemText(hItem);
	if(temp.Find(_T("添加")) == -1){
		//增加“添加****”子条目
		if(GetTreeCtrl().GetParentItem(hItem) == NULL && GetTreeCtrl().ItemHasChildren(hItem) == 0&&temp.Find(_T("关系")) == -1){
			CString subItemText = _T("添加");
			subItemText.Append(temp);
			GetTreeCtrl().InsertItem(subItemText, hItem);
		}
		GetDocument()->SetCurrentInfo(temp);
		GetDocument()->UpdateAllViews(this);
	}else{
		HTREEITEM hParentItem = GetTreeCtrl().GetParentItem(hItem);
		CString parentName = GetTreeCtrl().GetItemText(hParentItem);
		//弹出对话框,输入相应的数字
		int index = 0;
		for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
			if(parentName.Compare(Table[index].chineseName) == 0){
				break;
			}
		}
		if(Table[index].lpfCreateDlg != NULL)
			Table[index].lpfCreateDlg(GetDocument());
	}
	CTreeView::OnLButtonDblClk(nFlags, point);
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	//cs.dwExStyle |= TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_DISABLEDRAGDROP|TVS_SHOWSELALWAYS;
	return CTreeView::PreCreateWindow(cs);
}
