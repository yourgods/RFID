// mdiView.cpp : CmdiView ���ʵ��
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

// CmdiView ����/����

CmdiView::CmdiView()
{
	// TODO: �ڴ˴���ӹ������

}

CmdiView::~CmdiView()
{
}

BOOL CmdiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CListView::PreCreateWindow(cs);
}

void CmdiView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: ���� GetListCtrl() ֱ�ӷ��� ListView ���б�ؼ���
	//  �Ӷ������������ ListView��
}


// CmdiView ���

#ifdef _DEBUG
void CmdiView::AssertValid() const
{
	CListView::AssertValid();
}

void CmdiView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CmdiDoc* CmdiView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmdiDoc)));
	return (CmdiDoc*)m_pDocument;
}
#endif //_DEBUG


// CmdiView ��Ϣ�������
void CmdiView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: ��Ӵ�������Ӧ�û��Դ�����ͼ��ʽ�ĸ���	
	CListView::OnStyleChanged(nStyleType,lpStyleStruct);	
}

void CmdiView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: �ڴ����ר�ô����/����û���
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
