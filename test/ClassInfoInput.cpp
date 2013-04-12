// ClassInfoInput.cpp : 实现文件
//

#include "stdafx.h"
#include "test.h"
#include "ClassInfoInput.h"
#include "testDoc.h"
#include "database.h"
#include "common.h"
#include "mysql.h"
#include "mysql_com.h"

int ClassInfoCreateDlg(CtestDoc *doc)
{
	CClassInfoInput dlg;
	while(dlg.DoModal()==IDOK)
		doc->UpdateAllViews(NULL);
	return TRUE;
}

int ClassInfoQueryRight(CArray<rowItem, rowItem> &rest)
{
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].name.Compare(_T("class")) == 0)
			break;
	}

	struct query query;
	query.from = _T("class,school,school_class");
	query.otherTerm = _T("");
	query.selectedItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount + 1;
	int i = 0;
	for(i = 0; i<query.selectedItemCount - 2; i++){
		query.SI[i].name = _T("class.") + Table[index].fieldValue[i].fieldName;
		query.SI[i].chineseName = Table[index].fieldValue[i].chineseName;
	}
	query.SI[i].name = _T("school.name");
	query.SI[i].chineseName = _T("所属学校");
	i++;
	query.SI[i].name = _T("class.") + Table[index].fieldValue[Table[index].itemKeyCount + Table[index].itemOthersCount - 1].fieldName;
	query.SI[i].chineseName = Table[index].fieldValue[Table[index].itemKeyCount + Table[index].itemOthersCount - 1].chineseName;

	query.whereItemCount = 2;
	query.WI[0].name = _T("class.class_ID");
	query.WI[0].valueType = MYSQL_TYPE_LONG;
	query.WI[0].value = _T("school_class.class_ID");
	query.WI[1].name = _T("school.school_ID");
	query.WI[1].valueType = MYSQL_TYPE_LONG;
	query.WI[1].value = _T("school_class.school_ID");
	Query(&query, rest);

	if(rest.GetCount() == 0){
		rowItem temp;
		temp.fieldCount = Table[index].itemKeyCount + Table[index].itemOthersCount + 1;
		for(i = 0; i<query.selectedItemCount - 2; i++){
			temp.CI[i].name = _T("class.") + Table[index].fieldValue[i].fieldName;
			temp.CI[i].chineseName =  Table[index].fieldValue[i].chineseName;
			temp.CI[i].valueType = Table[index].fieldValue[i].fieldType;
			temp.CI[i].value = _T("");
		}
		temp.CI[i].name = _T("school.name");
		temp.CI[i].chineseName = _T("所属学校");
		temp.CI[i].valueType = MYSQL_TYPE_STRING;
		temp.CI[i].value = _T("");
		i++;
		temp.CI[i].name = _T("class.") + Table[index].fieldValue[Table[index].itemKeyCount + Table[index].itemOthersCount-1].fieldName;
		temp.CI[i].chineseName = Table[index].fieldValue[Table[index].itemKeyCount + Table[index].itemOthersCount-1].chineseName;
		temp.CI[i].valueType = Table[index].fieldValue[Table[index].itemKeyCount + Table[index].itemOthersCount-1].fieldType;
		temp.CI[i].value = _T("");
		rest.Add(temp);
	}
	return 0;
}

// CClassInfoInput 对话框

IMPLEMENT_DYNAMIC(CClassInfoInput, CDialog)

CClassInfoInput::CClassInfoInput(CWnd* pParent /*=NULL*/)
	: CDialog(CClassInfoInput::IDD, pParent)
	, class_ID(_T(""))
	, name(_T(""))
	, remark(_T(""))
	, school(_T(""))
	, school_index(0)
	, class_index(0)
	, m_strSchoolNo(_T(""))
	, m_strSchoolManager(_T(""))
	, m_strSchoolMobile(_T(""))
	, m_strSchoolFix(_T(""))
	, m_strSchoolRemark(_T(""))
{
	m_aSchool.RemoveAll();
	m_aClass.RemoveAll();
}

CClassInfoInput::~CClassInfoInput()
{
}

void CClassInfoInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NO, class_ID);
	DDX_Text(pDX, IDC_NAME, name);
	DDX_Text(pDX, IDC_EDIT3, remark);
	DDX_Text(pDX, IDC_SCHOOL, school);
	DDX_Text(pDX, IDC_NO2, m_strSchoolNo);
	DDX_Text(pDX, IDC_OFFICER2, m_strSchoolManager);
	DDX_Text(pDX, IDC_MOBI2, m_strSchoolMobile);
	DDX_Text(pDX, IDC_HOME2, m_strSchoolFix);
	DDX_Text(pDX, IDC_REMARK2, m_strSchoolRemark);
}


BEGIN_MESSAGE_MAP(CClassInfoInput, CDialog)
	ON_BN_CLICKED(IDOK, &CClassInfoInput::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CClassInfoInput::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_NAME, &CClassInfoInput::OnCbnSelchangeName)
	ON_CBN_SELCHANGE(IDC_SCHOOL, &CClassInfoInput::OnCbnSelchangeSchool)
END_MESSAGE_MAP()

// CClassInfoInput 消息处理程序

void CClassInfoInput::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_NO, class_ID);
	GetDlgItemText(IDC_NAME, name);
	GetDlgItemText(IDC_EDIT3, remark);
	GetDlgItemText(IDC_SCHOOL, school);
	GetDlgItemText(IDC_NO2, m_strSchoolNo);
	GetDlgItemText(IDC_OFFICER2, m_strSchoolManager);
	GetDlgItemText(IDC_MOBI2, m_strSchoolMobile);
	GetDlgItemText(IDC_HOME2, m_strSchoolFix);
	GetDlgItemText(IDC_REMARK2, m_strSchoolRemark);

	//更新学校信息数据
	int index = school_index;

	//查询出相应的学校信息
	struct query query;
	query.from  = Table[index].name;
	query.selectedItemCount = 2;
	query.SI[0].name = _T("school_ID");
	int temp_index = 0;
	for(temp_index = 0; temp_index<Table[index].itemKeyCount + Table[index].itemOthersCount; temp_index++){
		if(Table[index].fieldValue[temp_index].fieldName.Compare(query.SI[0].name) == 0)
			break;
	}
	query.SI[1].chineseName = Table[index].fieldValue[temp_index].chineseName;
	query.SI[1].name = _T("class_number");
	for(temp_index = 0; temp_index<Table[index].itemKeyCount + Table[index].itemOthersCount; temp_index++){
		if(Table[index].fieldValue[temp_index].fieldName.Compare(query.SI[1].name) == 0)
			break;
	}
	query.SI[1].chineseName = Table[index].fieldValue[temp_index].chineseName;
	query.whereItemCount = 0;
	query.otherTerm = _T("");
	CArray<rowItem, rowItem> school_rest;
	Query(&query, school_rest);
	int count = school_rest.GetCount();
	if(!count){
		MessageBox(_T("没有对应的学校,请先输入学校信息"));
		return;
	}
	
	//插入班级信息数据
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].chineseName.Compare(_T("班级信息")) == 0)
			break;
	}
	struct update update;
	update.from = _T("class");
	update.whereItemCount = 1;
	update.WI[0].name = _T("class_ID");
	for(temp_index = 0; temp_index<Table[index].itemKeyCount + Table[index].itemOthersCount; temp_index++){
		if(Table[index].fieldValue[temp_index].fieldName.Compare(update.WI[0].name) == 0)
			break;
	}
	update.WI[0].valueType = Table[index].fieldValue[temp_index].fieldType;
	update.WI[0].value = class_ID;
	update.setItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
	for(int i = 0; i<update.setItemCount; i++){
		update.SI[i].name = Table[index].fieldValue[i].fieldName;
		update.SI[i].valueType = Table[index].fieldValue[i].fieldType;
	}
	update.SI[0].value = class_ID;
	update.SI[1].value = name;
	update.SI[2].value = remark;
	if(Insert(&update) != -1){
		//更新学校信息,班级数加1
		struct update update;
		update.from = _T("school");
		//set class_number increment
		update.setItemCount = 1;
		update.SI[0].name = school_rest[0].CI[1].name;
		update.SI[0].valueType = school_rest[0].CI[1].valueType;
		CString t;
		t.Format(_T("%d"), _ttoi(school_rest[0].CI[1].value) + 1);
		update.SI[0].value = t;
		//set where term use school_ID;
		update.whereItemCount = 1;
		update.WI[0].name = school_rest[0].CI[0].name;
		update.WI[0].valueType = school_rest[0].CI[0].valueType;
		update.WI[0].value = school_rest[0].CI[0].value;
		InsertUniform(&update);

		//更新学校-班级关系表
		for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
			if(Table[index].chineseName.Compare(_T("学校班级关系")) == 0)
				break;
		}
		update.from = _T("school_class");
		update.setItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
		for(int i = 0; i<update.setItemCount; i++){
			update.SI[i].name = Table[index].fieldValue[i].fieldName;
			update.SI[i].valueType = Table[index].fieldValue[i].fieldType;
		}
		update.SI[0].value = class_ID;
		update.SI[1].value = school;
		update.SI[2].value = _T("");
		update.whereItemCount = 1;
		update.WI[0].name = update.SI[0].name;
		update.WI[0].valueType = update.SI[0].valueType;
		update.WI[0].value = update.SI[0].value;
		InsertUniform(&update);
	}else{
		InsertUniform(&update);
	}

	OnOK();
}

void CClassInfoInput::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CClassInfoInput::PreInitDialog()
{
	// TODO: 在此添加专用代码和/或调用基类
	SetDlgItemText(IDC_NO, class_ID);
	SetDlgItemText(IDC_NAME, name);
	SetDlgItemText(IDC_REMARK, remark);
	SetDlgItemText(IDC_SCHOOL, school);

	//Get All Class Information
	for(int i = 0; i<(sizeof(Table)/sizeof(Table[0])); i++){
		if(Table[i].name.Compare(_T("class")) == 0){
			class_index = i;
			break;
		}
	}
	struct query query;
	query.from = _T("class");
	query.otherTerm = _T("");
	query.selectedItemCount = Table[class_index].itemKeyCount + Table[class_index].itemOthersCount;
	for(int i =0 ;i<query.selectedItemCount; i++){
		query.SI[i].name = Table[class_index].fieldValue[i].fieldName;
		query.SI[i].chineseName = Table[class_index].fieldValue[i].chineseName;
	}
	query.whereItemCount = 0;
	Query(&query, m_aClass);
	CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_NAME);
	for(int i = 0; i<m_aClass.GetCount(); i++)
		pBox->AddString(m_aClass.GetAt(i).CI[1].value);

	//Get All School Information
	for(int i = 0; i<(sizeof(Table)/sizeof(Table[0])); i++){
		if(Table[i].name.Compare(_T("school")) == 0){
			school_index = i;
			break;
		}
	}
	query.from = _T("school");
	query.otherTerm = _T("");
	query.selectedItemCount = Table[school_index].itemKeyCount + Table[school_index].itemOthersCount;
	for(int i =0 ;i<query.selectedItemCount; i++){
		query.SI[i].name = Table[school_index].fieldValue[i].fieldName;
		query.SI[i].chineseName = Table[school_index].fieldValue[i].chineseName;
	}
	query.whereItemCount = 0;
	Query(&query, m_aSchool);
	pBox = (CComboBox *)GetDlgItem(IDC_SCHOOL);
	for(int i = 0; i<m_aSchool.GetCount(); i++)
	{
		pBox->AddString(m_aSchool.GetAt(i).CI[1].value);
	}

	CDialog::PreInitDialog();
}

void CClassInfoInput::OnCbnSelchangeName()
{
	// TODO: 在此添加控件通知处理程序代码
	CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_NAME);
	int nSelect = pBox->GetCurSel();
	CString strName;
	pBox->GetLBText(nSelect, strName);
	int class_temp = 0;
	for(int i = 0; i<m_aClass.GetCount(); i++){
		if(m_aClass.GetAt(i).CI[1].value.Compare(strName) == 0){
			class_temp = i;
			break;
		}
	}

	class_ID = m_aClass.GetAt(class_temp).CI[0].value;
	name = m_aClass.GetAt(class_temp).CI[1].value;
	remark = m_aClass.GetAt(class_temp).CI[2].value;

	//Get The relative school id
	int relative_index = 0;
	for(relative_index = 0; relative_index < (sizeof(Table)/sizeof(Table[0])); relative_index++){
		if(Table[relative_index].name.Compare(_T("school_class")) == 0)
			break;
	}
	struct query query;
	query.from = _T("school_class");
	query.otherTerm = _T("");
	query.selectedItemCount = Table[relative_index].itemKeyCount + Table[relative_index].itemOthersCount;
	for(int i = 0; i<query.selectedItemCount; i++){
		query.SI[i].name = Table[relative_index].fieldValue[i].fieldName;
		query.SI[i].chineseName = Table[relative_index].fieldValue[i].chineseName;
	}
	query.whereItemCount = 1;
	query.WI[0].name = Table[relative_index].fieldValue[0].fieldName;
	query.WI[0].valueType = Table[relative_index].fieldValue[0].fieldType;
	query.WI[0].value = class_ID;
	CArray<rowItem, rowItem> rest;
	Query(&query, rest);
	
	if(rest.GetCount() == 0){
		school = _T("");
		m_strSchoolNo = _T("");
		m_strSchoolManager = _T("");
		m_strSchoolMobile = _T("");
		m_strSchoolFix = _T("");
		m_strSchoolRemark = _T("");
	}else{
		m_strSchoolNo = rest.GetAt(0).CI[1].value;
		int school_temp = 0;
		for(school_temp = 0;school_temp<m_aSchool.GetCount(); school_temp++){
			if(m_aSchool.GetAt(school_temp).CI[0].value.Compare(m_strSchoolNo) == 0){
				break;
			}
		}
		if(school_temp >= m_aSchool.GetCount()){
			school = _T("");
			m_strSchoolNo = _T("");
			m_strSchoolManager = _T("");
			m_strSchoolMobile = _T("");
			m_strSchoolFix = _T("");
			m_strSchoolRemark = _T("");
		}else{
			school = m_aSchool.GetAt(school_temp).CI[1].value;
			m_strSchoolManager = m_aSchool.GetAt(school_temp).CI[2].value;
			m_strSchoolMobile = m_aSchool.GetAt(school_temp).CI[3].value;
			m_strSchoolFix = m_aSchool.GetAt(school_temp).CI[4].value;
			m_strSchoolRemark = m_aSchool.GetAt(school_temp).CI[5].value;
		}
	}


	SetDlgItemText(IDC_NO, class_ID);
	SetDlgItemText(IDC_NAME, name);
	SetDlgItemText(IDC_EDIT3, remark);
	SetDlgItemText(IDC_SCHOOL, school);
	SetDlgItemText(IDC_NO2, m_strSchoolNo);
	SetDlgItemText(IDC_OFFICER2, m_strSchoolManager);
	SetDlgItemText(IDC_MOBI2, m_strSchoolMobile);
	SetDlgItemText(IDC_HOME2, m_strSchoolFix);
	SetDlgItemText(IDC_REMARK2, m_strSchoolRemark);
}

void CClassInfoInput::OnCbnSelchangeSchool()
{
	// TODO: 在此添加控件通知处理程序代码
	CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_SCHOOL);
	int nSelect = pBox->GetCurSel();
	CString strName;
	pBox->GetLBText(nSelect, strName);
	int school_temp;
	for(school_temp = 0; school_temp<m_aSchool.GetCount(); school_temp++){
		if(m_aSchool.GetAt(school_temp).CI[1].value.Compare(strName) == 0)
			break;
	}
	m_strSchoolNo = m_aSchool.GetAt(school_temp).CI[0].value;
	school = m_aSchool.GetAt(school_temp).CI[1].value;
	m_strSchoolManager = m_aSchool.GetAt(school_temp).CI[2].value;
	m_strSchoolMobile = m_aSchool.GetAt(school_temp).CI[3].value;
	m_strSchoolFix = m_aSchool.GetAt(school_temp).CI[4].value;
	m_strSchoolRemark = m_aSchool.GetAt(school_temp).CI[5].value;
	SetDlgItemText(IDC_SCHOOL, school);
	SetDlgItemText(IDC_NO2, m_strSchoolNo);
	SetDlgItemText(IDC_OFFICER2, m_strSchoolManager);
	SetDlgItemText(IDC_MOBI2, m_strSchoolMobile);
	SetDlgItemText(IDC_HOME2, m_strSchoolFix);
	SetDlgItemText(IDC_REMARK2, m_strSchoolRemark);
}
