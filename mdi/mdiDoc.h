// mdiDoc.h : CmdiDoc 类的接口
//


#pragma once


class CmdiDoc : public CDocument
{
protected: // 仅从序列化创建
	CmdiDoc();
	DECLARE_DYNCREATE(CmdiDoc)

// 属性
public:
	CString m_currentInfo;

// 操作
public:
	bool QueryDB(CString &temp);
// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CmdiDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};


