// LeftView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Test1.h"
#include "LeftView.h"
#include "database.h"


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


// CLeftView ���

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
#endif //_DEBUG

CTest1Doc * CLeftView::GetDocument() // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTest1Doc)));
	return (CTest1Doc*)m_pDocument;
}

// CLeftView ��Ϣ�������

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
	
	// TODO: �ڴ����ר�ô����/����û���
	long lStyleOld = GetTreeCtrl().GetStyle();
	lStyleOld |=TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_DISABLEDRAGDROP|TVS_SHOWSELALWAYS;
	SetWindowLong(GetTreeCtrl().m_hWnd, GWL_STYLE, lStyleOld);
	GetTreeCtrl().SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);

	// TODO: ���� GetTreeCtrl() ֱ�ӷ��� TreeView �����ؼ���
	//  �Ӷ������������ TreeView��
	HTREEITEM hItem;
	for(int i =0 ; i<12; i++){
		hItem = GetTreeCtrl().InsertItem(Table[i].chineseName, TVI_ROOT);
		if(Table[i].chineseName.Find(_T("��ϵ")) == -1 && Table[i].lpfCreateDlg != NULL)
			GetTreeCtrl().InsertItem(_T("���") + Table[i].chineseName, hItem);
	}
}

void CLeftView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CString temp = GetTreeCtrl().GetItemText(hItem);
	if(temp.Find(_T("���")) == -1){
		//���ӡ����****������Ŀ
		int index = 0;
		for(index = 0; index < (sizeof(Table)/sizeof(Table[0])); index++){
			if(Table[index].chineseName.Compare(temp) == 0)
				break;
		}
		if(GetTreeCtrl().GetParentItem(hItem) == NULL && GetTreeCtrl().ItemHasChildren(hItem) == 0&&temp.Find(_T("��ϵ")) == -1&&Table[index].lpfCreateDlg != NULL){
			CString subItemText = _T("���");
			subItemText.Append(temp);
			GetTreeCtrl().InsertItem(subItemText, hItem);
		}
		GetDocument()->SetCurrentInfo(temp);
		GetDocument()->UpdateAllViews(this);
	}else{
		HTREEITEM hParentItem = GetTreeCtrl().GetParentItem(hItem);
		CString parentName = GetTreeCtrl().GetItemText(hParentItem);
		//�����Ի���,������Ӧ������
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
