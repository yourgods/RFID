// Test1Doc.h : CTest1Doc ��Ľӿ�
//


#pragma once


class CTest1Doc : public CDocument
{
protected: // �������л�����
	CTest1Doc();
	DECLARE_DYNCREATE(CTest1Doc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CTest1Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	CString m_strCurrentInfo;
	int SetCurrentInfo(CString currentInfo);
};


