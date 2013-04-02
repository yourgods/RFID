// serverDoc.cpp : CserverDoc 类的实现
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


// CserverDoc 构造/析构

CserverDoc::CserverDoc()
{
	// TODO: 在此添加一次性构造代码

}

CserverDoc::~CserverDoc()
{
}

BOOL CserverDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CserverDoc 序列化

void CserverDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CserverDoc 诊断

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


// CserverDoc 命令
