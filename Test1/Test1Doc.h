// Test1Doc.h : CTest1Doc 类的接口
//


#pragma once


class CTest1Doc : public CDocument
{
protected: // 仅从序列化创建
	CTest1Doc();
	DECLARE_DYNCREATE(CTest1Doc)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CTest1Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	CString m_strCurrentInfo;
	int SetCurrentInfo(CString currentInfo);
};


