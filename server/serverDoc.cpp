// serverDoc.cpp : CserverDoc ���ʵ��
//

#include "stdafx.h"
#include "server.h"

#include "serverDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CserverDoc

IMPLEMENT_DYNCREATE(CserverDoc, CDocument)

BEGIN_MESSAGE_MAP(CserverDoc, CDocument)
END_MESSAGE_MAP()


// CserverDoc ����/����

CserverDoc::CserverDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CserverDoc::~CserverDoc()
{
}

BOOL CserverDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CserverDoc ���л�

void CserverDoc::Serialize(CArchive& ar)
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


// CserverDoc ���

#ifdef _DEBUG
void CserverDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CserverDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CserverDoc ����
