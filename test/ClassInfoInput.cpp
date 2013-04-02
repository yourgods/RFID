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

int ClassInfoCreateDlg(CtestDoc *doc){
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
		temp.CI[i].name = _T("class.") + Table[index].fieldValue[Table[index].itemKeyCount + Table[index].itemOthersCount-1].fieldName;
		temp.CI[i].chineseName = Table[index].fieldValue[Table[index].itemKeyCount + Table[index].itemOthersCount-1].chineseName;
		temp.CI[i].valueType = MYSQL_TYPE_STRING;
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
{

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
END_MESSAGE_MAP()


#if 0
struct item{
	CString name;
	enum enum_field_types valueType;
	CString value;
};

struct update{
	CString name;
	int itemQuery;
	int itemUpdate;
	struct item itemValue[20];
};

struct fieldAttr{
	bool primaryKey;
	enum enum_field_types fieldType;
	CString fieldName;
	CString chineseName;
};

struct table{
	CString name;  //���ݿ����
	CString chineseName;
	int (*lpfCreateDlg)(CtestDoc *doc);
	int itemKeyCount; //��������
	int itemOthersCount; //�����и���
	struct fieldAttr fieldValue[15]; //���е�����
};

#endif

// CClassInfoInput ��Ϣ�������

void CClassInfoInput::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemText(IDC_NO, class_ID);
	GetDlgItemText(IDC_NAME, name);
	GetDlgItemText(IDC_REMARK, remark);
	GetDlgItemText(IDC_SCHOOL, school);

	//����ѧУ��Ϣ����
	int index;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].chineseName.Find(_T("ѧУ��Ϣ")) != -1)
			break;
	}

	//��ѯ����Ӧ��ѧУ��Ϣ
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
		MessageBox(_T("û�ж�Ӧ��ѧУ,��������ѧУ��Ϣ"));
		return;
	}
	
	//����༶��Ϣ����
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].chineseName.Compare(_T("�༶��Ϣ")) == 0)
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
		//����ѧУ��Ϣ,�༶����1
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

		//����ѧУ-�༶��ϵ��
		for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
			if(Table[index].chineseName.Compare(_T("ѧУ�༶��ϵ")) == 0)
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
	CDialog::PreInitDialog();
}
