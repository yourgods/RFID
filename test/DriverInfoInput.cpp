// DriverInfoInput.cpp : 实现文件
//

#include "stdafx.h"
#include "test.h"
#include "DriverInfoInput.h"
#include "testDoc.h"
#include "common.h"
#include "database.h"

int DriverInfoCreateDlg(CtestDoc *doc)
{
	CDriverInfoInput dlg;
	while(dlg.DoModal()==IDOK){
		doc->UpdateAllViews(NULL);
	}
	return TRUE;
}

int DriverInfoQueryRight(CArray<rowItem, rowItem> &rest)
{
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].name.Compare(_T("driver")) == 0)
			break;
	}

	struct query query;
	query.from = _T("driver");
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

// CDriverInfoInput 对话框

IMPLEMENT_DYNAMIC(CDriverInfoInput, CDialog)

CDriverInfoInput::CDriverInfoInput(CWnd* pParent /*=NULL*/)
	: CDialog(CDriverInfoInput::IDD, pParent)
	, m_strDriverID(_T(""))
	, m_strName(_T(""))
	, m_strSex(_T(""))
	, m_strAge(_T(""))
	, m_strMobile(_T(""))
	, m_strHome(_T(""))
	, m_strAddress(_T(""))
	, m_strRemark(_T(""))
{

}

CDriverInfoInput::~CDriverInfoInput()
{
}

void CDriverInfoInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NO, m_strDriverID);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_SEX, m_strSex);
	DDX_Text(pDX, IDC_AGE, m_strAge);
	DDX_Text(pDX, IDC_MOBILE, m_strMobile);
	DDX_Text(pDX, IDC_HOME, m_strHome);
	DDX_Text(pDX, IDC_ADDR, m_strAddress);
	DDX_Text(pDX, IDC_REMARK, m_strRemark);
}


BEGIN_MESSAGE_MAP(CDriverInfoInput, CDialog)
	ON_BN_CLICKED(IDOK, &CDriverInfoInput::OnBnClickedOk)
END_MESSAGE_MAP()


// CDriverInfoInput 消息处理程序

void CDriverInfoInput::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_NO, m_strDriverID);
	GetDlgItemText(IDC_NAME, m_strName);
	GetDlgItemText(IDC_SEX, m_strSex);
	GetDlgItemText(IDC_AGE, m_strAge);
	GetDlgItemText(IDC_MOBILE, m_strMobile);
	GetDlgItemText(IDC_HOME, m_strHome);
	GetDlgItemText(IDC_ADDR, m_strAddress);
	GetDlgItemText(IDC_REMARK, m_strRemark);

	//更新司机信息数据
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].chineseName.Compare(_T("司机信息")) == 0)
			break;
	}
	
	struct update update;
	update.from = _T("driver");
	update.whereItemCount = 1;
	update.WI[0].name = Table[index].fieldValue[0].fieldName;
	update.WI[0].valueType = Table[index].fieldValue[0].fieldType;
	update.WI[0].value = m_strDriverID;

	update.setItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
	for(int i = 0; i<update.setItemCount; i++){
		update.SI[i].name = Table[index].fieldValue[i].fieldName;
		update.SI[i].valueType = Table[index].fieldValue[i].fieldType;
	}
	update.SI[0].value = m_strDriverID;
	update.SI[1].value = m_strName;
	update.SI[2].value = m_strSex;
	update.SI[3].value = m_strAge;
	update.SI[4].value = m_strMobile;
	update.SI[5].value = m_strHome;
	update.SI[6].value = m_strAddress;
	update.SI[7].value = m_strRemark;

	InsertUniform(&update);

	OnOK();
}
