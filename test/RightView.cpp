// RightView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "test.h"
#include "RightView.h"
#include "database.h"
#include "testDoc.h"
#include "common.h"


// CRightView

IMPLEMENT_DYNCREATE(CRightView, CListView)

CRightView::CRightView()
{

}

CRightView::~CRightView()
{
}

BEGIN_MESSAGE_MAP(CRightView, CListView)
END_MESSAGE_MAP()


// CRightView ���

#ifdef _DEBUG
void CRightView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CRightView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif

CtestDoc* CRightView::GetDocument() // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CtestDoc)));
	return (CtestDoc*)m_pDocument;
}

#endif //_DEBUG


// CRightView ��Ϣ�������

void CRightView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	CString temp = GetDocument()->m_strCurrentInfo;
	int index = 0;
	for(index = 0; index<sizeof(Table)/sizeof(Table[0]); index++){
		if(temp.Compare(Table[index].chineseName) == 0)
			break;
	}

	//������������Ϣ
	GetListCtrl().DeleteAllItems();
	while(GetListCtrl().DeleteColumn(0));
	GetListCtrl().SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	CArray<rowItem, rowItem> rest;
	if(Table[index].lpfQueryRight)
		Table[index].lpfQueryRight(rest);

	int count = rest.GetCount();
	if(count != 0){
		for(int i = 0; i<rest[0].fieldCount; i++){
			if(i==rest[0].fieldCount-1){
				GetListCtrl().InsertColumn(i,rest[0].CI[i].chineseName,LVCFMT_LEFT);
				GetListCtrl().SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
			}
			else
				GetListCtrl().InsertColumn(i,rest[0].CI[i].chineseName,LVCFMT_LEFT, 80);
		}
	}else{
		for(int i = 0; i<Table[index].itemKeyCount + Table[index].itemOthersCount; i++){
			if(i==Table[index].itemKeyCount + Table[index].itemOthersCount-1){
				GetListCtrl().InsertColumn(i,Table[index].fieldValue[i].chineseName,LVCFMT_LEFT);
				GetListCtrl().SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
			}
			else
				GetListCtrl().InsertColumn(i,Table[index].fieldValue[i].chineseName,LVCFMT_LEFT, 80);
		}	
	}

	//�����ѯ�õ�������
	for(int row = 0; row < count; row++){ //��ѯ��������
		GetListCtrl().InsertItem(row,rest[row].CI[0].value);
		for(int i = 1; i<rest[0].fieldCount; i++){
			GetListCtrl().SetItemText(row, i, rest[row].CI[i].value);
		}
	}

	GetListCtrl().ShowWindow(TRUE);
	GetListCtrl().UpdateWindow();
}

void CRightView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: �ڴ����ר�ô����/����û���
	CString temp = GetDocument()->m_strCurrentInfo;
	int index = 0;
	for(index = 0; index<sizeof(Table)/sizeof(Table[0]); index++){
		if(temp.Compare(Table[index].chineseName) == 0)
			break;
	}

	//������������Ϣ
	GetListCtrl().DeleteAllItems();
	while(GetListCtrl().DeleteColumn(0));
	GetListCtrl().SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	CArray<rowItem, rowItem> rest;
	if(Table[index].lpfQueryRight)
		Table[index].lpfQueryRight(rest);

	int count = rest.GetCount();
	if(count != 0){
		for(int i = 0; i<rest[0].fieldCount; i++){
			if(i==rest[0].fieldCount-1){
				GetListCtrl().InsertColumn(i,rest[0].CI[i].chineseName,LVCFMT_LEFT);
				GetListCtrl().SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
			}
			else
				GetListCtrl().InsertColumn(i,rest[0].CI[i].chineseName,LVCFMT_LEFT, 80);
		}
	}else{
		for(int i = 0; i<Table[index].itemKeyCount + Table[index].itemOthersCount; i++){
			if(i==Table[index].itemKeyCount + Table[index].itemOthersCount-1){
				GetListCtrl().InsertColumn(i,Table[index].fieldValue[i].chineseName,LVCFMT_LEFT);
				GetListCtrl().SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
			}
			else
				GetListCtrl().InsertColumn(i,Table[index].fieldValue[i].chineseName,LVCFMT_LEFT, 80);
		}	
	}

	//�����ѯ�õ�������
	for(int row = 0; row < count; row++){ //��ѯ��������
		GetListCtrl().InsertItem(row,rest[row].CI[0].value);
		for(int i = 1; i<rest[0].fieldCount; i++){
			GetListCtrl().SetItemText(row, i, rest[row].CI[i].value);
		}
	}

	GetListCtrl().ShowWindow(TRUE);
	GetListCtrl().UpdateWindow();
}

BOOL CRightView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	cs.style |= LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}
