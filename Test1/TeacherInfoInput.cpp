// TeacherInfoInput.cpp : 实现文件
//

#include "stdafx.h"
#include "Test1.h"
#include "TeacherInfoInput.h"
#include "Test1Doc.h"
#include "common.h"
#include "database.h"

int TeacherInfoCreateDlg(CTest1Doc *doc)
{
	CTeacherInfoInput dlg;
	while(dlg.DoModal()==IDOK){
		doc->UpdateAllViews(NULL);
	}
	return TRUE;
}

int TeacherInfoQueryRight(CArray<rowItem, rowItem> &rest)
{
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].name.Compare(_T("teacher")) == 0)
			break;
	}

	struct query query;
	query.from = _T("teacher");
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

// CTeacherInfoInput 对话框

IMPLEMENT_DYNAMIC(CTeacherInfoInput, CDialog)

CTeacherInfoInput::CTeacherInfoInput(CWnd* pParent /*=NULL*/)
	: CDialog(CTeacherInfoInput::IDD, pParent)
	, m_strTeacherID(_T(""))
	, m_strName(_T(""))
	, m_strSex(_T(""))
	, m_strAge(_T(""))
	, m_strMobile(_T(""))
	, m_strHome(_T(""))
	, m_strAddress(_T(""))
	, m_strRemark(_T(""))
{

}

CTeacherInfoInput::~CTeacherInfoInput()
{
}

void CTeacherInfoInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NO, m_strTeacherID);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_SEX, m_strSex);
	DDX_Text(pDX, IDC_AGE, m_strAge);
	DDX_Text(pDX, IDC_MOBILE, m_strMobile);
	DDX_Text(pDX, IDC_HOME, m_strHome);
	DDX_Text(pDX, IDC_ADDR, m_strAddress);
	DDX_Text(pDX, IDC_REMARK, m_strRemark);
}


BEGIN_MESSAGE_MAP(CTeacherInfoInput, CDialog)
	ON_BN_CLICKED(IDOK, &CTeacherInfoInput::OnBnClickedOk)
END_MESSAGE_MAP()


// CTeacherInfoInput 消息处理程序

void CTeacherInfoInput::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_NO, m_strTeacherID);
	GetDlgItemText(IDC_NAME, m_strName);
	GetDlgItemText(IDC_SEX, m_strSex);
	GetDlgItemText(IDC_AGE, m_strAge);
	GetDlgItemText(IDC_MOBILE, m_strMobile);
	GetDlgItemText(IDC_HOME, m_strHome);
	GetDlgItemText(IDC_ADDR, m_strAddress);
	GetDlgItemText(IDC_REMARK, m_strRemark);

	//更新教师信息数据
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].chineseName.Compare(_T("教师信息")) == 0)
			break;
	}
	
	struct update update;
	update.from = _T("teacher");
	update.whereItemCount = 1;
	update.WI[0].name = Table[index].fieldValue[0].fieldName;
	update.WI[0].valueType = Table[index].fieldValue[0].fieldType;
	update.WI[0].value = m_strTeacherID;

	update.setItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
	for(int i = 0; i<update.setItemCount; i++){
		update.SI[i].name = Table[index].fieldValue[i].fieldName;
		update.SI[i].valueType = Table[index].fieldValue[i].fieldType;
	}
	update.SI[0].value = m_strTeacherID;
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
