// Test1Doc.cpp : CTest1Doc ���ʵ��
//

#include "stdafx.h"
#include "Test1.h"

#include "Test1Doc.h"
#include "common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTest1Doc

IMPLEMENT_DYNCREATE(CTest1Doc, CDocument)

BEGIN_MESSAGE_MAP(CTest1Doc, CDocument)
END_MESSAGE_MAP()


// CTest1Doc ����/����

CTest1Doc::CTest1Doc()
: m_strCurrentInfo(_T("ѧУ��Ϣ"))
{
	// TODO: �ڴ����һ���Թ������
	int err = 0;
	if(err = OpenDB()){
		CString temp;
		temp.Format(_T("%d"), err);
		MessageBox(NULL, _T("�����ݿ����,�����:") + temp, _T("�����ݿ�"), MB_OK);
	}else{
		//::MessageBox(NULL, _T("�����ݿ�ɹ�"), _T("�����ݿ�"), MB_OK);	
	}
}

CTest1Doc::~CTest1Doc()
{
	CloseDB();
}

BOOL CTest1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CTest1Doc ���л�

void CTest1Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CTest1Doc ���

#ifdef _DEBUG
void CTest1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTest1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTest1Doc ����

int CTest1Doc::SetCurrentInfo(CString currentInfo)
{
	m_strCurrentInfo = currentInfo;
	return 0;
}
