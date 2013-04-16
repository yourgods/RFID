// ClassInfoInput.cpp : 实现文件
//

#include "stdafx.h"
#include "Test1.h"
#include "ClassInfoInput.h"
#include "Test1Doc.h"
#include "mysql.h"
#include "mysql_com.h"
#include "database.h"

int ClassInfoCreateDlg(CTest1Doc *doc)
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
}


BEGIN_MESSAGE_MAP(CClassInfoInput, CDialog)
	ON_BN_CLICKED(IDOK, &CClassInfoInput::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CClassInfoInput::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_NAME, &CClassInfoInput::OnCbnSelchangeName)
END_MESSAGE_MAP()

// CClassInfoInput 消息处理程序

void CClassInfoInput::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_NO, class_ID);
	GetDlgItemText(IDC_NAME, name);
	GetDlgItemText(IDC_EDIT3, remark);
	GetDlgItemText(IDC_SCHOOL, school);

	//是否已存在班级
	int index_class = 0;
	for(index_class = 0; index_class<m_aClass.GetCount(); index_class++){
		if(m_aClass.GetAt(index_class).CI[0].value.Compare(class_ID) == 0)
			break;
	}

	if(index_class>=m_aClass.GetCount()){ //不存在此班级，则直接创建此班级，并将对应的学校的班级数递增
		struct update update;
		update.from = _T("class");
		update.setItemCount = Table[class_index].itemKeyCount + Table[class_index].itemOthersCount;
		for(int i = 0; i<update.setItemCount; i++){
			update.SI[i].name = Table[class_index].fieldValue[i].fieldName;
			update.SI[i].valueType = Table[class_index].fieldValue[i].fieldType;
		}
		update.SI[0].value = class_ID;
		update.SI[1].value = name;
		update.SI[2].value = remark;
		update.whereItemCount = 1;
		update.WI[0].name = update.SI[0].name;
		update.WI[0].valueType = update.SI[0].valueType;
		update.WI[0].value = update.SI[0].value;
		InsertUniform(&update);

		//更新学校班级数
		int index_school = 0;
		for(index_school = 0; index_school<m_aSchool.GetCount(); index_school++){
			if(m_aSchool.GetAt(index_school).CI[1].value.Compare(school) == 0)
				break;
		}
		update.from = _T("school");
		update.setItemCount = Table[school_index].itemKeyCount + Table[school_index].itemOthersCount;
		for(int i = 0; i<update.setItemCount; i++){
			update.SI[i].name = Table[school_index].fieldValue[i].fieldName;
			update.SI[i].valueType = Table[school_index].fieldValue[i].fieldType;
		}
		update.SI[0].value = m_aSchool.GetAt(index_school).CI[0].value;
		update.SI[1].value = m_aSchool.GetAt(index_school).CI[1].value;
		update.SI[2].value = m_aSchool.GetAt(index_school).CI[2].value;
		update.SI[3].value = m_aSchool.GetAt(index_school).CI[3].value;
		update.SI[4].value = m_aSchool.GetAt(index_school).CI[4].value;
		update.SI[5].value = m_aSchool.GetAt(index_school).CI[5].value;
		CString t;
		t.Format(_T("%d"), _ttoi(m_aSchool.GetAt(index_school).CI[6].value) + 1);
		update.SI[6].value = t;
		update.SI[7].value = m_aSchool.GetAt(index_school).CI[7].value;
		update.SI[8].value = m_aSchool.GetAt(index_school).CI[8].value;
		update.SI[9].value = m_aSchool.GetAt(index_school).CI[9].value;
		update.SI[10].value = m_aSchool.GetAt(index_school).CI[10].value;
		update.whereItemCount = 1;
		update.WI[0].name = update.SI[0].name;
		update.WI[0].valueType = update.SI[0].valueType;
		update.WI[0].value = update.SI[0].value;
		InsertUniform(&update);
	}else{ //此班级已存在
		//判断是否变更了学校
		int index_school_new = 0;
		for(index_school_new = 0; index_school_new<m_aSchool.GetCount(); index_school_new++){
			if(m_aSchool.GetAt(index_school_new).CI[1].value.Compare(school) == 0)
				break;
		}

		int sch_cla_index = 0;
		for(sch_cla_index = 0; sch_cla_index<(sizeof(Table)/sizeof(Table[0])); sch_cla_index++){
			if(Table[sch_cla_index].name.Compare(_T("school_class")) == 0)
				break;
		}
		struct query query;
		query.from = _T("school_class");
		query.otherTerm = _T("");
		query.selectedItemCount = Table[sch_cla_index].itemKeyCount + Table[sch_cla_index].itemOthersCount;
		for(int i = 0; i<query.selectedItemCount; i++){
			query.SI[i].name = Table[sch_cla_index].fieldValue[i].fieldName;
			query.SI[i].chineseName = Table[sch_cla_index].fieldValue[i].chineseName;
		}
		query.whereItemCount = 1;
		query.WI[0].name = Table[sch_cla_index].fieldValue[0].fieldName;
		query.WI[0].valueType = Table[sch_cla_index].fieldValue[0].fieldType;
		query.WI[0].value = class_ID;
		CArray<rowItem, rowItem> sch_cla_rest;
		Query(&query, sch_cla_rest);
		if(sch_cla_rest.GetAt(0).CI[1].value.Compare(m_aSchool.GetAt(index_school_new).CI[0].value) == 0){ //没有变更学校则不做任何操作
			struct update update;
			update.from = _T("class");
			update.setItemCount = Table[class_index].itemKeyCount + Table[class_index].itemOthersCount;
			for(int i = 0; i<update.setItemCount; i++){
				update.SI[i].name = Table[class_index].fieldValue[i].fieldName;
				update.SI[i].valueType = Table[class_index].fieldValue[i].fieldType;
			}
			update.SI[0].value = class_ID;
			update.SI[1].value = name;
			update.SI[2].value = remark;
			update.whereItemCount = 1;
			update.WI[0].name = update.SI[0].name;
			update.WI[0].valueType = update.SI[0].valueType;
			update.WI[0].value = update.SI[0].value;
			InsertUniform(&update);
		}else{
			CString strSchool_Old_ID = sch_cla_rest.GetAt(0).CI[1].value;
			CString strSchool_new_ID = m_aSchool.GetAt(index_school_new).CI[0].value;
			
			//获取班级学生人数 
			struct query query;
			query.from = _T("sch_tea_cla");
			query.otherTerm = _T("");
			query.selectedItemCount = 1;
			query.SI[0].name = _T("student_ID");
			query.SI[0].chineseName = _T("学生编号");
			query.whereItemCount = 1;
			query.WI[0].name = _T("class_ID");
			query.WI[0].valueType = MYSQL_TYPE_STRING;
			query.WI[0].value = class_ID;
			CArray<rowItem, rowItem> num_rest;
			Query(&query, num_rest);
			int stu_num = num_rest.GetCount();
			num_rest.RemoveAll();

			//获取教师人数
			query.from = _T("sch_tea_cla");
			query.otherTerm = _T("");
			query.selectedItemCount = 1;
			query.SI[0].name = _T("teacher_ID");
			query.SI[0].chineseName = _T("教师编号");
			query.whereItemCount = 1;
			query.WI[0].name = _T("class_ID");
			query.WI[0].valueType = MYSQL_TYPE_STRING;
			query.WI[0].value = class_ID;
			Query(&query, num_rest);
			int tea_num = num_rest.GetCount();
			num_rest.RemoveAll();

			//更新原属学校信息
			int index_school_old = 0;
			for(index_school_old = 0; index_school_old<m_aSchool.GetCount(); index_school_old++){
				if(m_aSchool.GetAt(index_school_old).CI[0].value.Compare(strSchool_Old_ID) == 0)
					break;
			}
			struct update update;
			update.from = _T("school");
			update.setItemCount = m_aSchool.GetAt(index_school_old).fieldCount;
			for(int i = 0; i<update.setItemCount; i++){
				update.SI[i].name =m_aSchool.GetAt(index_school_old).CI[i].name;
				update.SI[i].valueType = m_aSchool.GetAt(index_school_old).CI[i].valueType;
				update.SI[i].value = m_aSchool.GetAt(index_school_old).CI[i].value;
			}
			CString t;
			t.Format(_T("%d"), (_ttoi(update.SI[5].value) - stu_num)<0?0:_ttoi(update.SI[5].value) - stu_num);
			update.SI[5].value = t;
			t.Format(_T("%d"), (_ttoi(update.SI[6].value) - 1)<0?0:_ttoi(update.SI[6].value) - 1);
			update.SI[6].value = t;
			t.Format(_T("%d"), (_ttoi(update.SI[7].value) - tea_num)<0?0:_ttoi(update.SI[7].value) - tea_num);
			update.SI[7].value = t;
			update.whereItemCount = 1;
			update.WI[0].name = update.SI[0].name;
			update.WI[0].valueType = update.SI[0].valueType;
			update.WI[0].value = update.SI[0].value;
			InsertUniform(&update);

			//更新先所属学校
			update.from = _T("school");
			update.setItemCount = m_aSchool.GetAt(index_school_new).fieldCount;
			for(int i = 0; i<update.setItemCount; i++){
				update.SI[i].name =m_aSchool.GetAt(index_school_new).CI[i].name;
				update.SI[i].valueType = m_aSchool.GetAt(index_school_new).CI[i].valueType;
				update.SI[i].value = m_aSchool.GetAt(index_school_new).CI[i].value;
			}
			t.Format(_T("%d"), _ttoi(update.SI[5].value) + stu_num);
			update.SI[5].value = t;
			t.Format(_T("%d"), _ttoi(update.SI[6].value) + 1);
			update.SI[6].value = t;
			t.Format(_T("%d"), _ttoi(update.SI[7].value) + tea_num);
			update.SI[7].value = t;
			update.whereItemCount = 1;
			update.WI[0].name = update.SI[0].name;
			update.WI[0].valueType = update.SI[0].valueType;
			update.WI[0].value = update.SI[0].value;
			InsertUniform(&update);

			//更新学校班级关系
			update.from = _T("school_class");
			update.setItemCount = Table[sch_cla_index].itemKeyCount + Table[sch_cla_index].itemOthersCount;
			for(int i = 0; i<update.setItemCount; i++){
				update.SI[i].name = Table[sch_cla_index].fieldValue[i].fieldName;
				update.SI[i].valueType = Table[sch_cla_index].fieldValue[i].fieldType;
			}
			update.SI[0].value = class_ID;
			update.SI[1].value = strSchool_new_ID;
			update.SI[2].value = _T("");
			update.whereItemCount = 1;
			update.WI[0].name = update.SI[0].name;
			update.WI[0].valueType = update.SI[0].valueType;
			update.WI[0].value = update.SI[0].value;
			InsertUniform(&update);
		}
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
	
	UpdateClassInfo();

	//Get All School Information
	for(int i = 0; i<(sizeof(Table)/sizeof(Table[0])); i++){
		if(Table[i].name.Compare(_T("school")) == 0){
			school_index = i;
			break;
		}
	}

	UpdateSchoolInfo();

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
	}else{
		CString school_ID = rest.GetAt(0).CI[1].value;
		int school_temp = 0;
		for(school_temp = 0; school_temp<m_aSchool.GetCount(); school_temp++){
			if(m_aSchool.GetAt(school_temp).CI[0].value.Compare(school_ID) == 0)
				break;
		}
		CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_SCHOOL);
		pBox->SetCurSel(school_temp);
	}

	SetDlgItemText(IDC_NO, class_ID);
	SetDlgItemText(IDC_EDIT3, remark);
}

void CClassInfoInput::UpdateClassInfo(){
	struct query query;
	query.from = _T("class");
	query.otherTerm = _T("");
	query.selectedItemCount = Table[class_index].itemKeyCount + Table[class_index].itemOthersCount;
	for(int i =0 ;i<query.selectedItemCount; i++){
		query.SI[i].name = Table[class_index].fieldValue[i].fieldName;
		query.SI[i].chineseName = Table[class_index].fieldValue[i].chineseName;
	}
	query.whereItemCount = 0;
	m_aClass.RemoveAll();
	Query(&query, m_aClass);
	CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_NAME);
	while(pBox->GetCount()) pBox->DeleteString(0);
	for(int i = 0; i<m_aClass.GetCount(); i++)
		pBox->AddString(m_aClass.GetAt(i).CI[1].value);
}

void CClassInfoInput::UpdateSchoolInfo(){
	struct query query;
	query.from = _T("school");
	query.otherTerm = _T("");
	query.selectedItemCount = Table[school_index].itemKeyCount + Table[school_index].itemOthersCount;
	for(int i =0 ;i<query.selectedItemCount; i++){
		query.SI[i].name = Table[school_index].fieldValue[i].fieldName;
		query.SI[i].chineseName = Table[school_index].fieldValue[i].chineseName;
	}
	query.whereItemCount = 0;
	m_aSchool.RemoveAll();
	Query(&query, m_aSchool);
	CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_SCHOOL);
	while(pBox->GetCount()) pBox->DeleteString(0);
	for(int i = 0; i<m_aSchool.GetCount(); i++)
		pBox->AddString(m_aSchool.GetAt(i).CI[1].value);
}