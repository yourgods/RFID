// Test1Doc.cpp : CTest1Doc 类的实现
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


// CTest1Doc 构造/析构

CTest1Doc::CTest1Doc()
: m_strCurrentInfo(_T("学校信息"))
{
	// TODO: 在此添加一次性构造代码
	int err = 0;
	if(err = OpenDB()){
		CString temp;
		temp.Format(_T("%d"), err);
		MessageBox(NULL, _T("打开数据库错误,错误号:") + temp, _T("打开数据库"), MB_OK);
	}else{
		//::MessageBox(NULL, _T("打开数据库成功"), _T("打开数据库"), MB_OK);	
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

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CTest1Doc 序列化

void CTest1Doc::Serialize(CArchive& ar)
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


// CTest1Doc 诊断

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


// CTest1Doc 命令

int CTest1Doc::SetCurrentInfo(CString currentInfo)
{
	m_strCurrentInfo = currentInfo;
	return 0;
}
