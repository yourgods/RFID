// mdiDoc.h : CmdiDoc ��Ľӿ�
//


#pragma once


class CmdiDoc : public CDocument
{
protected: // �������л�����
	CmdiDoc();
	DECLARE_DYNCREATE(CmdiDoc)

// ����
public:
	CString m_currentInfo;

// ����
public:
	bool QueryDB(CString &temp);
// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CmdiDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


