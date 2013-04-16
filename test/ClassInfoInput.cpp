// ClassInfoInput.cpp : ʵ���ļ�
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
	query.SI[i].chineseName = _T("����ѧУ");
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
		temp.CI[i].chineseName = _T("����ѧУ");
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

// CClassInfoInput �Ի���

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
	ON_CBN_SELCHANGE(IDC_SCHOOL, &CClassInfoInput::OnCbnSelchangeSchool)
END_MESSAGE_MAP()

// CClassInfoInput ��Ϣ�������

void CClassInfoInput::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	GetDlgItemText(IDC_NO, class_ID);
//	GetDlgItemText(IDC_NAME, name);
//	GetDlgItemText(IDC_EDIT3, remark);
//	GetDlgItemText(IDC_SCHOOL, school);
//
//	//�ж�ѧУ�Ƿ����
//	BOOL school_exist = FALSE;
//	int index_school = 0;
//	for(index_school = 0; index_school<m_aSchool.GetCount(); index_school++){
//		if(m_aSchool.GetAt(index_school).CI[0].value.Compare(m_strSchoolNo) == 0)
//			break;
//	}
//	if(index_school < m_aSchool.GetCount()){ //ѧУ����
//		school_exist = TRUE;
//	}
//
//
//	//�Ƿ��Ѵ��ڰ༶
//	int index_class = 0;
//	for(index_class = 0; index_class<m_aClass.GetCount(); index_class++){
//		if(m_aClass.GetAt(index_class).CI[0].value.Compare(class_ID) == 0)
//			break;
//	}
//	if(index_class >= m_aClass.GetCount()){ //Ҫ����İ༶������
//		if(school_exist){ //ѧУ���ڣ������ѧУѧУ�İ༶��
//			struct update update;
//			update.from = _T("school");
//			update.setItemCount = 1;
//			update.SI[0].name = _T("class_number");
//			int t;
//			for(t = 0; t<Table[school_index].itemKeyCount + Table[school_index].itemOthersCount; t++){
//				if(Table[school_index].fieldValue[t].fieldName.Compare(update.SI[0].name) == 0)
//					break;
//			}
//			update.SI[0].valueType = Table[school_index].fieldValue[t].fieldType;
//			CString t1;
//			t1.Format(_T("%d"), _ttoi(m_aSchool.GetAt(index_school).CI[t].value) + 1);
//			update.SI[0].value = t1;
//			update.whereItemCount = 1;
//			update.WI[0].name = _T("school_ID");
//			for(t = 0; t<Table[school_index].itemKeyCount + Table[school_index].itemOthersCount; t++){
//				if(Table[school_index].fieldValue[t].fieldName.Compare(update.WI[0].name) == 0)
//					break;
//			}
//			update.WI[0].valueType = Table[school_index].fieldValue[t].fieldType;
//			update.WI[0].value = m_strSchoolNo;
//			InsertUniform(&update);
//		}else{ //ѧУ�����ڣ��򴴽�ѧУ
//			struct update update;
//			update.from = _T("school");
//			update.setItemCount = Table[school_index].itemKeyCount + Table[school_index].itemOthersCount;
//			for(int i = 0; i<update.setItemCount; i++){
//				update.SI[i].name = Table[school_index].fieldValue[i].fieldName;
//				update.SI[i].valueType = Table[school_index].fieldValue[i].fieldType;
//			}
//			update.SI[0].value = m_strSchoolNo;
//			update.SI[1].value = school;
//			update.SI[2].value = m_strSchoolManager;
//			update.SI[3].value = m_strSchoolMobile;
//			update.SI[4].value = m_strSchoolFix;
//			update.SI[5].value = _T("0");
//			update.SI[6].value = _T("1");
//			update.SI[7].value = _T("0");
//			update.SI[8].value = _T("0");
//			update.SI[9].value = _T("0");
//			update.SI[10].value = m_strSchoolRemark;
//			InsertUniform(&update);
//		}		
//	}else{ //Ҫ����İ༶���ڣ����������е�λ��Ϊindex_class
//		//�༶�������Ȳ�ѯ�༶������ѧ���ͽ�ʦ����
//		struct query query;
//		query.from = _T("sch_tea_cla");
//		query.otherTerm = _T("");
//		query.selectedItemCount = 1;
//		query.SI[0].name = _T("student_ID");
//		query.SI[0].chineseName = _T("ѧ�����");
//		query.whereItemCount = 1;
//		query.WI[0].name = _T("class_ID");
//		query.WI[0].valueType = MYSQL_TYPE_STRING;
//		query.WI[0].value = class_ID;
//		CArray<rowItem, rowItem> num_rest;
//		Query(&query, num_rest);
//		int stu_num = num_rest.GetCount();
//
//		query.selectedItemCount = 1;
//		query.SI[0].name = _T("teacher_ID");
//		query.SI[0].chineseName = _T("��ʦ���");
//		num_rest.RemoveAll();
//		Query(&query, num_rest);
//		int teacher_num = num_rest.GetCount();
//
//		if(!school_exist){ //ѧУ������,����ѧУ
//			struct update update;
//			update.from = _T("school");
//			update.setItemCount = Table[school_index].itemKeyCount + Table[school_index].itemOthersCount;
//			for(int i = 0; i<update.setItemCount; i++){
//				update.SI[i].name = Table[school_index].fieldValue[i].fieldName;
//				update.SI[i].valueType = Table[school_index].fieldValue[i].fieldType;
//			}
//			update.SI[0].value = m_strSchoolNo;
//			update.SI[1].value = school;
//			update.SI[2].value = m_strSchoolManager;
//			update.SI[3].value = m_strSchoolMobile;
//			update.SI[4].value = m_strSchoolFix;
//			CString t;
//			t.Format(_T("%d"), stu_num);
//			update.SI[5].value = t;
//			update.SI[6].value = _T("1");
//			t.Format(_T("%d"), teacher_num);
//			update.SI[7].value = t;
//			update.SI[8].value = _T("0");
//			update.SI[9].value = _T("0");
//			update.SI[10].value = m_strSchoolRemark;
//			InsertUniform(&update);
//		}else{ //ѧУ����
//			//���Ƿ��޸�������ѧУ
//			struct query query;
//			query.from = _T("school_class");
//			query.otherTerm = _T("");
//			query.selectedItemCount = 2;
//			query.SI[0].name = _T("class_ID");
//			query.SI[0].chineseName = _T("�༶���");
//			query.SI[1].name = _T("school_ID");
//			query.SI[1].chineseName = _T("ѧУ���");
//			query.whereItemCount = 1;
//			query.WI[0].name = _T("class_ID");
//			query.WI[0].valueType = MYSQL_TYPE_STRING;
//			query.WI[0].value = class_ID;
//			CArray<rowItem, rowItem> relative_rest;
//			Query(&query, relative_rest);
//			if(relative_rest.GetCount() != 0){ //�༶��ǰָ��ѧУ
//				if(relative_rest.GetAt(0).CI[0].value.Compare(m_strSchoolNo) == 0)
//					goto END;
//				else{
//					
//				}
//			}
//			struct update update;
//			update.from = _T("school");
//			update.setItemCount = 3;
//			update.SI[0].name = _T("student_number");
//			update.SI[0].valueType = MYSQL_TYPE_LONG;
//			update.SI[1].name = _T("class_number");
//			update.SI[1].valueType = MYSQL_TYPE_LONG;
//			update.SI[2].name = _T("teacher_number");
//			update.SI[2].valueType = MYSQL_TYPE_LONG;
//			CString tt;
//			tt.Format(_T("%d"), _ttoi(m_aSchool.GetAt(index_class).CI[5].value) + 1);
//			update.SI[0].value = tt;
//			tt.Format(_T("%d"), _ttoi(m_aSchool.GetAt(index_class).CI[6].value) + 1);
//			update.SI[1].value = tt;
//			tt.Format(_T("%d"), _ttoi(m_aSchool.GetAt(index_class).CI[7].value) + 1);
//			update.SI[2].value = tt;
//			update.whereItemCount = 1;
//			update.WI[0].name = _T("school_ID");
//			update.WI[0].valueType = MYSQL_TYPE_STRING;
//			update.WI[0].value = m_strSchoolNo;
//			InsertUniform(&update);
//		}
//	}
//END:
	OnOK();
}

void CClassInfoInput::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CClassInfoInput::PreInitDialog()
{
	// TODO: �ڴ����ר�ô����/����û���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	SetDlgItemText(IDC_NO, class_ID);
	SetDlgItemText(IDC_NAME, name);
	SetDlgItemText(IDC_EDIT3, remark);
	SetDlgItemText(IDC_SCHOOL, school);
}

void CClassInfoInput::OnCbnSelchangeSchool()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_SCHOOL);
	int nSelect = pBox->GetCurSel();
	CString strName;
	pBox->GetLBText(nSelect, strName);
	int school_temp;
	for(school_temp = 0; school_temp<m_aSchool.GetCount(); school_temp++){
		if(m_aSchool.GetAt(school_temp).CI[1].value.Compare(strName) == 0)
			break;
	}
	school = m_aSchool.GetAt(school_temp).CI[1].value;
	SetDlgItemText(IDC_SCHOOL, school);
}
