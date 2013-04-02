// mdiDoc.cpp : CmdiDoc ���ʵ��
//

#include "stdafx.h"
#include "mdi.h"
#include "common.h"
#include "mdiDoc.h"
#include "database.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern struct _DATABASE_ENTRY_ DBTable[]=
{
	_T("ѧУ��Ϣ"), SchoolInfoQuery, SchoolInfoInsertUniform, SchoolInfoInsert,
	_T(""), NULL, NULL, NULL
};

#define DIM(x) (sizeof(x)/sizeof(x[0]))

// CmdiDoc

IMPLEMENT_DYNCREATE(CmdiDoc, CDocument)

BEGIN_MESSAGE_MAP(CmdiDoc, CDocument)
END_MESSAGE_MAP()


// CmdiDoc ����/����

CmdiDoc::CmdiDoc()
{
	// TODO: �ڴ����һ���Թ������
	//�����ݿ�
	m_currentInfo = _T("");
	int err = 0;
	if(err = OpenDB()){
		CString temp;
		temp.Format(_T("%d"), err);
		::MessageBox(NULL, _T("�����ݿ����,�����:") + temp, _T("�����ݿ�"), MB_OK);
	}else{
		//::MessageBox(NULL, _T("�����ݿ�ɹ�"), _T("�����ݿ�"), MB_OK);	
	}
}

CmdiDoc::~CmdiDoc()
{
	CloseDB();
}

BOOL CmdiDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CmdiDoc ���л�

void CmdiDoc::Serialize(CArchive& ar)
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

bool CmdiDoc::QueryDB(CString &temp)
{
	for(int i = 0; i<DIM(DBTable); i++){
		if(DBTable[i].name == temp){ //�����ҵ��Ĳ�ѯ����
			struct results *res = NULL;
			int count = 0;
			if(!DBTable[i].lpfQuery(_T(""), &res, &count))//��ѯʧ��
			{
				return false;
			}else{
				free(res);
				return true;
			}
			break;
		}
	}
	return false;
}



// CmdiDoc ���

#ifdef _DEBUG
void CmdiDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CmdiDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CmdiDoc ����
