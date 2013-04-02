// LeftView.cpp : CLeftView ���ʵ��
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


// CLeftView ����/����

CLeftView::CLeftView()
{
	// TODO: �ڴ˴���ӹ������
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CTreeView::PreCreateWindow(cs);
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: ���� GetTreeCtrl() ֱ�ӷ��� TreeView �����ؼ���
	//  �Ӷ������������ TreeView��
	HTREEITEM hItem;
	hItem = GetTreeCtrl().InsertItem(_T("ѧУ��Ϣ"), TVI_ROOT);
	hItem = GetTreeCtrl().InsertItem(_T("�༶��Ϣ"), TVI_ROOT, hItem);
	hItem = GetTreeCtrl().InsertItem(_T("ѧ����Ϣ"), TVI_ROOT, hItem);
	hItem = GetTreeCtrl().InsertItem(_T("��ʦ��Ϣ"), TVI_ROOT, hItem);
	hItem = GetTreeCtrl().InsertItem(_T("�ҳ���Ϣ"), TVI_ROOT, hItem);
	hItem = GetTreeCtrl().InsertItem(_T("У����Ϣ"), TVI_ROOT, hItem);
	hItem = GetTreeCtrl().InsertItem(_T("˾����Ϣ"), TVI_ROOT, hItem);
	hItem = GetTreeCtrl().InsertItem(_T("У����·��Ϣ"), TVI_ROOT, hItem);
	hItem = GetTreeCtrl().InsertItem(_T("վ����Ϣ"), TVI_ROOT, hItem);
}


// CLeftView ���

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CmdiDoc* CLeftView::GetDocument() // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmdiDoc)));
	return (CmdiDoc*)m_pDocument;
}
#endif //_DEBUG


// CLeftView ��Ϣ�������

void CLeftView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CString temp = GetTreeCtrl().GetItemText(hItem);
	//���ӡ����****������Ŀ
	if(GetTreeCtrl().GetParentItem(hItem) == NULL && GetTreeCtrl().ItemHasChildren(hItem) == 0){
		CString subItemText = _T("���");
		subItemText.Append(temp);
		GetTreeCtrl().InsertItem(subItemText, hItem);
	}
	GetDocument()->m_currentInfo = temp;
	GetDocument()->UpdateAllViews(this);
	//if(!GetDocument()->QueryDB(temp)){
	//	MessageBox(_T("��ѯ���ݿ����:") + temp);
	//}else{
	//	MessageBox(_T("��ѯ���ݿ�ɹ�:") + temp);	
	//}
	CTreeView::OnLButtonDblClk(nFlags, point);
}

void CLeftView::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		//��ѯ���ݿⲢ��ʾ��ѯ������Ϣ
	//Sleep(100);
	//HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	//CString temp = GetTreeCtrl().GetItemText(hItem);
	//if(!QueryDB(temp)){
	//	MessageBox(_T("��ѯ���ݿ����:") + temp);
	//}else{
	//	MessageBox(_T("��ѯ���ݿ�ɹ�:") + temp);	
	//}
	*pResult = 0;
}
