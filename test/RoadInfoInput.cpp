// RoadInfoInput.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "test.h"
#include "RoadInfoInput.h"
#include "common.h"
#include "database.h"

int RoadInfoCreateDlg(CtestDoc *doc)
{
	CRoadInfoInput dlg;
	while(dlg.DoModal()==IDOK){
		doc->UpdateAllViews(NULL);
	}
	return TRUE;
}

int RoadInfoQueryRight(CArray<rowItem, rowItem> &rest)
{
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].name.Compare(_T("road")) == 0)
			break;
	}

	struct query query;
	query.from = _T("road");
	query.selectedItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
	for(int i = 0; i<query.selectedItemCount; i++){
		query.SI[i].name = Table[index].fieldValue[i].fieldName;
		query.SI[i].chineseName = Table[index].fieldValue[i].chineseName;
	}
	query.whereItemCount = 0;
	query.otherTerm = _T("");
	Query(&query, rest);
	if(rest.GetCount() == 0){
		rowItem temp;
		temp.fieldCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
		for(int i = 0; i<temp.fieldCount; i++){
			temp.CI[i].chineseName = Table[index].fieldValue[i].chineseName;
			temp.CI[i].name = Table[index].fieldValue[i].fieldName;
			temp.CI[i].valueType = Table[index].fieldValue[i].fieldType;
		}
		rest.Add(temp);
	}

	return TRUE;
}


// CRoadInfoInput �Ի���

IMPLEMENT_DYNAMIC(CRoadInfoInput, CDialog)

CRoadInfoInput::CRoadInfoInput(CWnd* pParent /*=NULL*/)
	: CDialog(CRoadInfoInput::IDD, pParent)
	, m_strRoadID(_T(""))
	, m_strName(_T(""))
	, m_strStart(_T(""))
	, m_strEnd(_T(""))
	, m_strRemark(_T(""))
{

}

CRoadInfoInput::~CRoadInfoInput()
{
}

void CRoadInfoInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strRoadID);
	DDX_Text(pDX, IDC_EDIT2, m_strName);
	DDX_Text(pDX, IDC_EDIT3, m_strStart);
	DDX_Text(pDX, IDC_EDIT4, m_strEnd);
	DDX_Text(pDX, IDC_EDIT5, m_strRemark);
}


BEGIN_MESSAGE_MAP(CRoadInfoInput, CDialog)
	ON_BN_CLICKED(IDOK, &CRoadInfoInput::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRoadInfoInput::OnBnClickedCancel)
END_MESSAGE_MAP()


// CRoadInfoInput ��Ϣ�������

void CRoadInfoInput::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemText(IDC_EDIT1, m_strRoadID);
	GetDlgItemText(IDC_EDIT2, m_strName);
	GetDlgItemText(IDC_EDIT3, m_strStart);
	GetDlgItemText(IDC_EDIT4, m_strEnd);
	GetDlgItemText(IDC_EDIT5, m_strRemark);

	//����У����·��Ϣ����
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].chineseName.Compare(_T("У����·��Ϣ")) == 0)
			break;
	}
	
	struct update update;
	update.from = _T("road");
	update.whereItemCount = 1;
	update.WI[0].name = Table[index].fieldValue[0].fieldName;
	update.WI[0].valueType = Table[index].fieldValue[0].fieldType;
	update.WI[0].value = m_strRoadID;

	update.setItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
	for(int i = 0; i<update.setItemCount; i++){
		update.SI[i].name = Table[index].fieldValue[i].fieldName;
		update.SI[i].valueType = Table[index].fieldValue[i].fieldType;
	}
	update.SI[0].value = m_strRoadID;
	update.SI[1].value = m_strName;
	update.SI[2].value = m_strStart;
	update.SI[3].value = m_strEnd;
	update.SI[4].value = m_strRemark;

	InsertUniform(&update);


	OnOK();
}

void CRoadInfoInput::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}
