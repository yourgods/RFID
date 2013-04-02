// mdiDoc.cpp : CmdiDoc 类的实现
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
	_T("学校信息"), SchoolInfoQuery, SchoolInfoInsertUniform, SchoolInfoInsert,
	_T(""), NULL, NULL, NULL
};

#define DIM(x) (sizeof(x)/sizeof(x[0]))

// CmdiDoc

IMPLEMENT_DYNCREATE(CmdiDoc, CDocument)

BEGIN_MESSAGE_MAP(CmdiDoc, CDocument)
END_MESSAGE_MAP()


// CmdiDoc 构造/析构

CmdiDoc::CmdiDoc()
{
	// TODO: 在此添加一次性构造代码
	//打开数据库
	m_currentInfo = _T("");
	int err = 0;
	if(err = OpenDB()){
		CString temp;
		temp.Format(_T("%d"), err);
		::MessageBox(NULL, _T("打开数据库错误,错误号:") + temp, _T("打开数据库"), MB_OK);
	}else{
		//::MessageBox(NULL, _T("打开数据库成功"), _T("打开数据库"), MB_OK);	
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

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CmdiDoc 序列化

void CmdiDoc::Serialize(CArchive& ar)
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

bool CmdiDoc::QueryDB(CString &temp)
{
	for(int i = 0; i<DIM(DBTable); i++){
		if(DBTable[i].name == temp){ //调用找到的查询函数
			struct results *res = NULL;
			int count = 0;
			if(!DBTable[i].lpfQuery(_T(""), &res, &count))//查询失败
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



// CmdiDoc 诊断

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


// CmdiDoc 命令
