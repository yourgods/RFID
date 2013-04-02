// BusInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "test.h"
#include "BusInfo.h"
#include "common.h"
#include "database.h"

int BusInfoCreateDlg(CtestDoc *doc)
{
	CBusInfoInput dlg;
	while(dlg.DoModal()==IDOK){
		doc->UpdateAllViews(NULL);
	}
	return TRUE;
}

int BusInfoQueryRight(CArray<rowItem, rowItem> &rest)
{
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].name.Compare(_T("bus")) == 0)
			break;
	}

	struct query query;
	query.from = _T("bus");
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
// CBusInfo 对话框

IMPLEMENT_DYNAMIC(CBusInfoInput, CDialog)

CBusInfoInput::CBusInfoInput(CWnd* pParent /*=NULL*/)
	: CDialog(CBusInfoInput::IDD, pParent)
	, m_strType(_T(""))
	, m_strBusno(_T(""))
	, m_strCap(_T(""))
	, m_strRemark(_T(""))
	, m_strBuyTime(_T(""))
	, m_strCP(_T(""))
{
	//ctlCalendar = new CMonthCalCtrl;
}

CBusInfoInput::~CBusInfoInput()
{
}

void CBusInfoInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strType);
	DDX_Text(pDX, IDC_EDIT3, m_strBusno);
	DDX_Text(pDX, IDC_EDIT4, m_strCap);
	DDX_Text(pDX, IDC_EDIT6, m_strRemark);
	DDX_Text(pDX, IDC_EDIT5, m_strBuyTime);
	DDX_Text(pDX, IDC_EDIT1, m_strCP);
}


BEGIN_MESSAGE_MAP(CBusInfoInput, CDialog)
	ON_EN_SETFOCUS(IDC_EDIT5, &CBusInfoInput::OnEnSetfocusEdit5)
	ON_BN_CLICKED(IDOK, &CBusInfoInput::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT5, &CBusInfoInput::OnEnKillfocusEdit5)
END_MESSAGE_MAP()


// CBusInfo 消息处理程序

void CBusInfoInput::OnEnSetfocusEdit5()
{
	// TODO: 在此添加控件通知处理程序代码
	/*ctlCalendar->ShowWindow(TRUE);
	ctlCalendar->do*/
	//ctlCalendar->UpdateWindow();
	
}

BOOL CBusInfoInput::OnInitDialog()
{
	CDialog::OnInitDialog();

	//// TODO:  在此添加额外的初始化
	//ctlCalendar->Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER,CPoint(20, 20), this, 0x224);
	//ctlCalendar->ShowWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CBusInfoInput::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT1, m_strCP);
	GetDlgItemText(IDC_EDIT2, m_strType);
	GetDlgItemText(IDC_EDIT3, m_strBusno);
	GetDlgItemText(IDC_EDIT4, m_strCap);
	GetDlgItemText(IDC_EDIT6, m_strRemark);
	GetDlgItemText(IDC_EDIT5, m_strBuyTime);

	//更新校车信息数据
	int index = 0;
	for(index = 0; index<(sizeof(Table)/sizeof(Table[0])); index++){
		if(Table[index].chineseName.Compare(_T("校车信息")) == 0)
			break;
	}
	
	struct update update;
	update.from = _T("bus");
	update.whereItemCount = 1;
	update.WI[0].name = Table[index].fieldValue[0].fieldName;
	update.WI[0].valueType = Table[index].fieldValue[0].fieldType;
	update.WI[0].value = m_strCP;

	update.setItemCount = Table[index].itemKeyCount + Table[index].itemOthersCount;
	for(int i = 0; i<update.setItemCount; i++){
		update.SI[i].name = Table[index].fieldValue[i].fieldName;
		update.SI[i].valueType = Table[index].fieldValue[i].fieldType;
	}
	update.SI[0].value = m_strCP;
	update.SI[1].value = m_strType;
	update.SI[2].value = m_strBusno;
	update.SI[3].value = m_strCap;
	update.SI[4].value = m_strBuyTime;
	update.SI[5].value = m_strRemark;

	InsertUniform(&update);
	OnOK();
}

void CBusInfoInput::OnEnKillfocusEdit5()
{
	// TODO: 在此添加控件通知处理程序代码
	//ctlCalendar->ShowWindow(FALSE);
	//ctlCalendar->UpdateWindow();
}
