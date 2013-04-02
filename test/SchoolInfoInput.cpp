// SchoolInfoInput.cpp : 实现文件
//

#include "stdafx.h"
#include "test.h"
#include "SchoolInfoInput.h"
#include "common.h"
#include "database.h"
#include "testDoc.h"

int SchoolInfoCreateDlg(CtestDoc *doc){
	CSchoolInfoInput dlg;
	while(dlg.DoModal()==IDOK){
		doc->UpdateAllViews(NULL);
	}
	return TRUE;
}

int SchooInfoQueryRight(CArray<rowItem, rowItem> &rest){
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].name.Compare(_T("school")) == 0)
			break;
	}
	struct query query;
	query.from = _T("school");
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
		temp.fieldCount = query.selectedItemCount;
		for(int i = 0; i<temp.fieldCount; i++){
			temp.CI[i].chineseName = query.SI[i].chineseName;
			temp.CI[i].value = _T("");
		}
		rest.Add(temp);
	}

	return TRUE;
}

// CSchoolInfoInput 对话框

IMPLEMENT_DYNAMIC(CSchoolInfoInput, CDialog)

CSchoolInfoInput::CSchoolInfoInput(CWnd* pParent /*=NULL*/)
	: CDialog(CSchoolInfoInput::IDD, pParent)
	, school_ID(_T(""))
	, name(_T(""))
	, manager(_T(""))
	, mobi_phone(_T(""))
	, fix_phone(_T(""))
	, remark(_T(""))
{

}

CSchoolInfoInput::~CSchoolInfoInput()
{
}

void CSchoolInfoInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NO, school_ID);
	DDX_Text(pDX, IDC_NAME, name);
	DDX_Text(pDX, IDC_OFFICER, manager);
	DDX_Text(pDX, IDC_MOBI, mobi_phone);
	DDX_Text(pDX, IDC_HOME, fix_phone);
	DDX_Text(pDX, IDC_REMARK, remark);
}


BEGIN_MESSAGE_MAP(CSchoolInfoInput, CDialog)
	ON_BN_CLICKED(IDOK2, &CSchoolInfoInput::OnBnClickedOk2)
	ON_BN_CLICKED(IDCANCEL2, &CSchoolInfoInput::OnBnClickedCancel2)
END_MESSAGE_MAP()


// CSchoolInfoInput 消息处理程序
void CSchoolInfoInput::OnBnClickedOk2()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_NO,school_ID);
	GetDlgItemText(IDC_NAME, name);
	GetDlgItemText(IDC_OFFICER, manager);
	GetDlgItemText(IDC_MOBI, mobi_phone);
	GetDlgItemText(IDC_HOME, fix_phone);
	GetDlgItemText(IDC_REMARK, remark);

	//更新学校信息数据
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].chineseName.Compare(_T("学校信息")) == 0)
			break;
	}
	struct update update;
	update.from = _T("school");

	update.whereItemCount = 1;
	update.WI[0].name = Table[index].fieldValue[0].fieldName;
	update.WI[0].valueType = Table[index].fieldValue[0].fieldType;
	update.WI[0].value = school_ID;

	update.setItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
	for(int i = 0; i<update.setItemCount; i++){
		update.SI[i].name = Table[index].fieldValue[i].fieldName;
		update.SI[i].valueType = Table[index].fieldValue[i].fieldType;
	}

	update.SI[0].value = school_ID;
	update.SI[1].value = name;
	update.SI[2].value = manager;
	update.SI[3].value = mobi_phone;
	update.SI[4].value = fix_phone;
	update.SI[5].value = _T("0");
	update.SI[6].value = _T("0");
	update.SI[7].value = _T("0");
	update.SI[8].value = _T("0");
	update.SI[9].value = _T("0");
	update.SI[10].value = remark;
	InsertUniform(&update);

	OnOK();
	
}

void CSchoolInfoInput::OnBnClickedCancel2()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

BOOL CSchoolInfoInput::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialog::PreCreateWindow(cs);
}

void CSchoolInfoInput::PreInitDialog()
{
	// TODO: 在此添加专用代码和/或调用基类
	SetDlgItemText(IDC_NO,school_ID);
	SetDlgItemText(IDC_NAME, name);
	SetDlgItemText(IDC_OFFICER, manager);
	SetDlgItemText(IDC_MOBI, mobi_phone);
	SetDlgItemText(IDC_HOME, fix_phone);
	SetDlgItemText(IDC_REMARK, remark); 
	CDialog::PreInitDialog();
}
