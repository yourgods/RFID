// serverDoc.h : CserverDoc ��Ľӿ�
//


#pragma once


class CserverDoc : public CDocument
{
protected: // �������л�����
	CserverDoc();
	DECLARE_DYNCREATE(CserverDoc)

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
	virtual ~CserverDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


