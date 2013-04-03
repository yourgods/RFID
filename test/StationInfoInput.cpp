// StationInfoInput.cpp : 实现文件
//

#include "stdafx.h"
#include "test.h"
#include "StationInfoInput.h"
#include "common.h"
#include "database.h"
#include "mysql.h"
#include "mysql_com.h"

int StationInfoCreateDlg(CtestDoc *doc)
{
	CStationInfoInput dlg;
	while(dlg.DoModal()==IDOK)
		doc->UpdateAllViews(NULL);
	return TRUE;
}

int StationInfoQueryRight(CArray<rowItem, rowItem> &rest)
{
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].name.Compare(_T("station")) == 0)
			break;
	}

	struct query query;
	query.from = _T("road, station, road_station");
	query.otherTerm = _T("");
	query.selectedItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount + 1;
	int i = 0;
	for(i = 0; i<query.selectedItemCount - 2; i++){
		query.SI[i].name = _T("station.") + Table[index].fieldValue[i].fieldName;
		query.SI[i].chineseName = Table[index].fieldValue[i].chineseName;
	}
	query.SI[i].name = _T("road.name");
	query.SI[i].chineseName = _T("所属线路");
	i++;
	query.SI[i].name = _T("station.") + Table[index].fieldValue[Table[index].itemKeyCount + Table[index].itemOthersCount - 1].fieldName;
	query.SI[i].chineseName = Table[index].fieldValue[Table[index].itemKeyCount + Table[index].itemOthersCount - 1].chineseName;
	query.whereItemCount = 2;
	query.WI[0].name = _T("road.road_ID");
	query.WI[0].valueType = MYSQL_TYPE_LONG;
	query.WI[0].value = _T("road_station.road_ID");
	query.WI[1].name = _T("station.station_ID");
	query.WI[1].valueType = MYSQL_TYPE_LONG;
	query.WI[1].value = _T("road_station.station_ID");
	Query(&query, rest);
	if(rest.GetCount() == 0){
		rowItem temp;
		temp.fieldCount = Table[index].itemKeyCount + Table[index].itemOthersCount + 1;
		for(i = 0; i<query.selectedItemCount - 2; i++){
			temp.CI[i].name = _T("station.") + Table[index].fieldValue[i].fieldName;
			temp.CI[i].chineseName =  Table[index].fieldValue[i].chineseName;
			temp.CI[i].valueType = Table[index].fieldValue[i].fieldType;
			temp.CI[i].value = _T("");
		}
		temp.CI[i].name = _T("road.name");
		temp.CI[i].chineseName = _T("所属线路");
		temp.CI[i].valueType = MYSQL_TYPE_STRING;
		temp.CI[i].value = _T("");
		i++;
		temp.CI[i].name = _T("station.") + Table[index].fieldValue[Table[index].itemKeyCount + Table[index].itemOthersCount-1].fieldName;
		temp.CI[i].chineseName = Table[index].fieldValue[Table[index].itemKeyCount + Table[index].itemOthersCount-1].chineseName;
		temp.CI[i].valueType = Table[index].fieldValue[Table[index].itemKeyCount + Table[index].itemOthersCount-1].fieldType;
		temp.CI[i].value = _T("");
		rest.Add(temp);
	}
	return 0;
}



// CStationInfoInput 对话框

IMPLEMENT_DYNAMIC(CStationInfoInput, CDialog)

CStationInfoInput::CStationInfoInput(CWnd* pParent /*=NULL*/)
	: CDialog(CStationInfoInput::IDD, pParent)
	, m_strStationID(_T(""))
	, m_strName(_T(""))
	, m_strRoadID(_T(""))
	, m_strRemark(_T(""))
{

}

CStationInfoInput::~CStationInfoInput()
{
}

void CStationInfoInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strStationID);
	DDX_Text(pDX, IDC_EDIT2, m_strName);
	DDX_Text(pDX, IDC_EDIT3, m_strRoadID);
	DDX_Text(pDX, IDC_EDIT4, m_strRemark);
}


BEGIN_MESSAGE_MAP(CStationInfoInput, CDialog)
	ON_BN_CLICKED(IDOK, &CStationInfoInput::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CStationInfoInput::OnBnClickedCancel)
END_MESSAGE_MAP()


// CStationInfoInput 消息处理程序

void CStationInfoInput::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT1, m_strStationID);
	GetDlgItemText(IDC_EDIT2, m_strName);
	GetDlgItemText(IDC_EDIT3, m_strRoadID);
	GetDlgItemText(IDC_EDIT4, m_strRemark);

	//更新校车线路信息数据
	int index;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].chineseName.Compare(_T("校车线路信息")) == 0)
			break;
	}

	//查询出相应的校车线路信息
	struct query query;
	query.from  = Table[index].name;
	query.selectedItemCount = 0;
	query.whereItemCount = 1;
	query.WI[0].name = Table[index].fieldValue[0].fieldName;
	query.WI[0].valueType = Table[index].fieldValue[0].fieldType;
	query.WI[0].value = m_strRoadID;
	CArray<rowItem, rowItem> road_rest;
	Query(&query, road_rest);
	int count = road_rest.GetCount();
	if(!count){
		MessageBox(_T("没有对应的线路,请先输入校车线路信息"));
		return;
	}
	
	//插入站点信息数据
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].chineseName.Compare(_T("站点信息")) == 0)
			break;
	}
	struct update update;
	update.from = _T("station");
	update.whereItemCount = 1;
	update.WI[0].name = _T("station_ID");
	update.WI[0].valueType = Table[index].fieldValue[0].fieldType;
	update.WI[0].value = m_strStationID;
	update.setItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
	for(int i = 0; i<update.setItemCount; i++){
		update.SI[i].name = Table[index].fieldValue[i].fieldName;
		update.SI[i].valueType = Table[index].fieldValue[i].fieldType;
	}
	update.SI[0].value = m_strStationID;
	update.SI[1].value = m_strName;;
	update.SI[2].value = m_strRemark;
	InsertUniform(&update);

	//插入线路站点关系信息
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].chineseName.Compare(_T("线路站点关系")) == 0)
			break;
	}
	update.from = _T("road_station");
	update.whereItemCount = 1;
	update.WI[0].name = Table[index].fieldValue[0].fieldName;
	update.WI[0].valueType = Table[index].fieldValue[0].fieldType;
	update.WI[0].value = m_strStationID;
	update.setItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
	for(int i = 0; i<update.setItemCount; i++){
		update.SI[i].name = Table[index].fieldValue[i].fieldName;
		update.SI[i].valueType = Table[index].fieldValue[i].fieldType;
	}
	update.SI[0].value = m_strStationID;
	update.SI[1].value = m_strRoadID;
	update.SI[2].value = m_strRemark;
	InsertUniform(&update);

	OnOK();
}

void CStationInfoInput::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
