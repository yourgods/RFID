// StudentInfoInput.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Test1.h"
#include "StudentInfoInput.h"
#include "common.h"
#include "database.h"

int StudentInfoCreateDlg(CTest1Doc *doc)
{
	CStudentInfoInput dlg;
	while(dlg.DoModal()==IDOK){
		doc->UpdateAllViews(NULL);
	}
	return TRUE;
}

int StudentInfoQueryRight(CArray<rowItem, rowItem> &rest)
{
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].name.Compare(_T("student")) == 0)
			break;
	}

	struct query query;
	query.from = _T("student");
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
// CStudentInfoInput �Ի���

IMPLEMENT_DYNAMIC(CStudentInfoInput, CDialog)

CStudentInfoInput::CStudentInfoInput(CWnd* pParent /*=NULL*/)
	: CDialog(CStudentInfoInput::IDD, pParent)
	, m_strStudentID(_T(""))
	, m_strName(_T(""))
	, m_strSex(_T(""))
	, m_strAge(_T(""))
	, m_strnroll(_T(""))
	, m_strAddress(_T(""))
	, m_strParentID(_T(""))
	, m_strTeacher(_T(""))
	, m_strRemark(_T(""))
	, m_strClass(_T(""))
{

}

CStudentInfoInput::~CStudentInfoInput()
{
}

void CStudentInfoInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NO, m_strStudentID);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_SEX, m_strSex);
	DDX_Text(pDX, IDC_AGE, m_strAge);
	DDX_Text(pDX, IDC_MOBILE, m_strnroll);
	DDX_Text(pDX, IDC_ADDR, m_strAddress);
	DDX_Text(pDX, IDC_PARENT, m_strParentID);
	DDX_Text(pDX, IDC_TEACHER, m_strTeacher);
	DDX_Text(pDX, IDC_REMARK, m_strRemark);
	DDX_Text(pDX, IDC_CLASS, m_strClass);
}


BEGIN_MESSAGE_MAP(CStudentInfoInput, CDialog)
	ON_BN_CLICKED(IDOK, &CStudentInfoInput::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CStudentInfoInput::OnBnClickedCancel)
END_MESSAGE_MAP()


// CStudentInfoInput ��Ϣ�������

void CStudentInfoInput::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemText(IDC_NO, m_strStudentID);
	GetDlgItemText(IDC_NAME, m_strName);
	GetDlgItemText(IDC_SEX, m_strSex);
	GetDlgItemText(IDC_AGE, m_strAge);
	GetDlgItemText(IDC_MOBILE, m_strnroll);
	GetDlgItemText(IDC_ADDR, m_strAddress);
	GetDlgItemText(IDC_PARENT, m_strParentID);
	GetDlgItemText(IDC_TEACHER, m_strTeacher);
	GetDlgItemText(IDC_CLASS, m_strClass);
	GetDlgItemText(IDC_REMARK, m_strRemark);

	//���ҳ���Ϣ�Ƿ����
	int parent_index = 0;
	for(parent_index = 0; parent_index<(sizeof(Table)/sizeof(Table[0])); parent_index++){
		if(Table[parent_index].chineseName.Compare(_T("�ҳ���Ϣ")) == 0)
			break;
	}
	struct query query;
	query.from = _T("parent");
	query.selectedItemCount = 0;
	query.whereItemCount = 1;
	query.WI[0].name = Table[parent_index].fieldValue[0].fieldName;
	query.WI[0].valueType = Table[parent_index].fieldValue[0].fieldType;
	query.WI[0].value = m_strParentID;
	query.otherTerm = _T("");
	CArray<rowItem, rowItem> parent_rest;
	Query(&query, parent_rest, false);
	if(parent_rest.GetCount() == 0){
		MessageBox(_T("δ�ҵ���Ӧ�ļҳ���Ϣ,���������Ӧ�ļҳ���Ϣ��������ѧ����Ϣ!"));
		return;
	}

	//����ʦ��Ϣ�Ƿ����
	int teacher_index = 0;
	for(teacher_index = 0; teacher_index<(sizeof(Table)/sizeof(Table[0])); teacher_index++){
		if(Table[teacher_index].chineseName.Compare(_T("��ʦ��Ϣ")) == 0)
			break;
	}
	query.from = _T("teacher");
	query.selectedItemCount = 0;
	query.whereItemCount = 1;
	query.WI[0].name = Table[teacher_index].fieldValue[0].fieldName;
	query.WI[0].valueType = Table[teacher_index].fieldValue[0].fieldType;
	query.WI[0].value = m_strTeacher;
	query.otherTerm = _T("");
	CArray<rowItem, rowItem> teacher_rest;
	Query(&query, teacher_rest, false);
	if(teacher_rest.GetCount() == 0){
		MessageBox(_T("δ�ҵ���Ӧ�Ľ�ʦ��Ϣ,���������Ӧ�ļҳ���Ϣ��������ѧ����Ϣ!"));
		return;
	}

	//���༶��Ϣ�Ƿ����
	int class_index = 0;
	for(class_index = 0; class_index<(sizeof(Table)/sizeof(Table[0])); class_index++){
		if(Table[class_index].chineseName.Compare(_T("�༶��Ϣ")) == 0)
			break;
	}
	query.from = _T("class");
	query.selectedItemCount = 0;
	query.whereItemCount = 1;
	query.WI[0].name = Table[class_index].fieldValue[0].fieldName;
	query.WI[0].valueType = Table[class_index].fieldValue[0].fieldType;
	query.WI[0].value = m_strClass;
	query.otherTerm = _T("");
	CArray<rowItem, rowItem> class_rest;
	Query(&query, class_rest, false);
	if(class_rest.GetCount() == 0){
		MessageBox(_T("δ�ҵ���Ӧ�İ༶��Ϣ,���������Ӧ�ļҳ���Ϣ��������ѧ����Ϣ!"));
		return;
	}

	//����ѧ����Ϣ����
	int student_index = 0;
	for(student_index = 0; student_index<(sizeof(Table)/sizeof(Table[0])); student_index++){
		if(Table[student_index].chineseName.Compare(_T("ѧ����Ϣ")) == 0)
			break;
	}
	
	struct update update;
	update.from = _T("student");
	update.whereItemCount = 1;
	update.WI[0].name = Table[student_index].fieldValue[0].fieldName;
	update.WI[0].valueType = Table[student_index].fieldValue[0].fieldType;
	update.WI[0].value = m_strStudentID;

	update.setItemCount = Table[student_index].itemKeyCount + Table[student_index].itemOthersCount;
	for(int i = 0; i<update.setItemCount; i++){
		update.SI[i].name = Table[student_index].fieldValue[i].fieldName;
		update.SI[i].valueType = Table[student_index].fieldValue[i].fieldType;
	}
	update.SI[0].value = m_strStudentID;
	update.SI[1].value = m_strName;
	update.SI[2].value = m_strSex;
	update.SI[3].value = m_strAge;
	update.SI[4].value = m_strnroll;
	update.SI[5].value = m_strAddress;
	update.SI[6].value = m_strRemark;
	InsertUniform(&update);

	//����ѧ���ҳ���ϵ��Ϣ
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].chineseName.Compare(_T("ѧ���ҳ���ϵ")) == 0)
			break;
	}
	update.from = _T("stu_par");
	update.whereItemCount = 1;
	update.WI[0].name = Table[index].fieldValue[0].fieldName;
	update.WI[0].valueType = Table[index].fieldValue[0].fieldType;
	update.WI[0].value = m_strStudentID;
	update.setItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
	for(int i = 0; i<update.setItemCount; i++){
		update.SI[i].name = Table[index].fieldValue[i].fieldName;
		update.SI[i].valueType = Table[index].fieldValue[i].fieldType;
	}
	update.SI[0].value = m_strStudentID;
	update.SI[1].value = m_strParentID;
	update.SI[2].value = _T("��ĸ");
	update.SI[3].value = m_strRemark;
	InsertUniform(&update);

	//����ѧ����ʦ�༶��ϵ
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].chineseName.Compare(_T("ѧ����ʦ�༶��ϵ")) == 0)
			break;
	}
	update.from = _T("stu_tea_cla");
	update.whereItemCount = 1;
	update.WI[0].name = Table[index].fieldValue[0].fieldName;
	update.WI[0].valueType = Table[index].fieldValue[0].fieldType;
	update.WI[0].value = m_strStudentID;
	update.setItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
	for(int i = 0; i<update.setItemCount; i++){
		update.SI[i].name = Table[index].fieldValue[i].fieldName;
		update.SI[i].valueType = Table[index].fieldValue[i].fieldType;
	}
	update.SI[0].value = m_strStudentID;
	update.SI[1].value = m_strClass;
	update.SI[2].value = m_strTeacher;
	update.SI[3].value = m_strRemark;
	InsertUniform(&update);

	OnOK();
}

void CStudentInfoInput::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}
