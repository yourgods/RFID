#include "stdafx.h"
#include "RFID.h"
#include "config.h"
#include "steganogram.h"
#include "tools.h"
#include "RFID prototypeDlg.h"
#include "RFID prototype.h"
#include "sms.h"
#include "Resource.h"
#include "message.h"
#include "database.h"
#include "string.h"
#include "stdio.h"

#define RFID_TEST

extern CRFIDprototypeApp theApp;

static HANDLE hStr;
static unsigned char rx_data[1024];
static OVERLAPPED m_osRead, m_osWrite; // 用于重叠读/写

static BOOL initVar()
{
	HANDLE hStr = theApp.hStr;
	return TRUE;
}

//UART接收线程函数
DWORD RXINT_Thread(PVOID pArg)
{

	initVar();
	//unsigned char temp_data[1024];
	DWORD length;
	CString	str1;
	CRFIDprototypeDlg *pWnd = (CRFIDprototypeDlg *)pArg;

	while(1)
	{
		CString miyao,codetemp;
		char mingwen[28];
		unsigned int keybox[7][10];
		unsigned int dekeybox[7][10];
		ReadFile(hStr,rx_data,240,&length,&m_osRead); 
		codetemp=rx_data;
		miyao=LogisticAlgorithm();
		key(miyao,keybox);
		de_key(keybox,dekeybox);
		uncode(codetemp,mingwen,dekeybox);
#ifdef RFID_TEST
		if(1)
#else
		if(length)
#endif	
		{
			//Got Phone No., student name & student ID no.
#ifndef RFID_TEST
			str1= mingwen;
			CString strTemp,strTemp2;
			char ctemp[34]={'\0'};			
			for (int i=0;i<17;i++)
			{
				ctemp[i*2] = (rx_data[i]>>4)&0x0F;
				ctemp[i*2+1] = rx_data[i]&0x0F;
			}
			char ch11[12];
			for (int n=16;n<27;n++)
			{
				ch11[n-16] = Hex2Char(ctemp[n]);
			}
			ch11[11] = '\0';
			CString strPhone;
			strPhone = ch11;
#else
			CString strPhone;
			strPhone = _T("18656563791");
#endif
			//AfxMessageBox(strPhone);

#ifndef RFID_TEST
			char ch6[13];
			for (int n=4;n<16;n++)
			{
				ch6[n-4] = Hex2Char(ctemp[n]);
			}	
			ch6[12]='\0';

			CString input;
			input = ch6;
			CString strName;
			// CString to char*
			int strlen1 = input.GetLength();
			char* str = new char[strlen1+1];
			memset(str, 0, strlen1+1);
			for(int i=0; i<strlen1; i++)	str[i] = input.GetAt(i);
			// hex string to wchar_t*
			int wchslen = 0;
			if(HexStr2WChars(str, strlen1, 4, NULL, &wchslen) != -1){
				wchar_t* wchs = new wchar_t[wchslen+1];
				memset(wchs, 0, sizeof(wchar_t)*(wchslen+1));
				int n = HexStr2WChars(str, strlen1, 4, wchs, &wchslen);
				if(n != -1){
					// wchar_t to CString
					wchar_t* pwch = wchs;
					for(int i=0; i<wchslen; i++)	strName.AppendChar(*pwch++);
				}
				if(wchs)	delete []wchs;
			}
#else
			CString strName;
			strName = _T("印凤行");
#endif

			//Read Student ID
#ifndef RFID_TEST
			//....
			CString strStudentID;
#else
			CString strStudentID = _T("stud_01");
#endif
			//Check either up or down bus
			if(!pWnd->CheckDataExist(strName,strPhone))//说明上车
			{
				StructPerson structTemp;
				structTemp.strName = strName;
				structTemp.strID = strStudentID;
				structTemp.strPhoneNum = strPhone;
				structTemp.strUpStation = pWnd->m_strCurrentStation;
				CTime Upt=CTime::GetCurrentTime();
				CString strUpTime = Upt.Format(L"%Y-%m-%d %H:%M:%S");
				structTemp.strUpTime = strUpTime;
				structTemp.strDownStation = _T("");
				structTemp.strDownTime = _T("");
				structTemp.strUpMessage = "否";
				structTemp.strDownMessage = "否";
				strArray.Add(structTemp);
				
				//判断是否超载
				int itest = _ttoi(iniFile.m_strCount);
				if(strArray.GetCount()>itest)
				{
					//PlaySound(L"\\ResidentFlash\\Sound\\Full.wav",NULL,SND_SYNC|SND_NODEFAULT);
					//发送报警短信
					CString strContent,strCenterNum,strPhoneEnd;
					strCenterNum = SMSC;
					strContent.Format(_T("%d"),strArray.GetCount());
					strContent=_T("30,@该车已超载，承载人数为") + iniFile.m_strCount + _T(",实载") + strContent;
					strPhoneEnd=_T("86")+iniFile.m_strPhoneNum;

					SM_PARAM sm_param;
					USES_CONVERSION;

					CHAR* pch=W2A(strContent);
					strcpy(sm_param.TP_UD, pch);

					pch=W2A(strPhoneEnd);
					strcpy(sm_param.TPA, pch);

					pch=W2A(strCenterNum);
					strcpy(sm_param.SCA, pch);

					sm_param.TP_PID = 0;
					sm_param.TP_DCS = GSM_UCS2;
#ifdef USE_TRAFFIC
					pWnd->gsm->PutSendMessage(&sm_param);
#else
					::gsmSendMessage(&sm_param);
#endif	
				}

			}
			else//下车
			{
				CTime Downt=CTime::GetCurrentTime();
				CString strDownTime = Downt.Format(L"%Y-%m-%d %H:%M:%S");
				StructPerson stTemp;
				for (int j=0;j<strArray.GetCount();j++)
				{
					stTemp = strArray.GetAt(j);
					if(strName==stTemp.strName&&strPhone==stTemp.strPhoneNum&&strStudentID==stTemp.strID)
					{
						stTemp.strDownStation = pWnd->m_strCurrentStation;
						stTemp.strDownTime = strDownTime;
						strArray.SetAt(j,stTemp);
						break;
					}
				}
				//DONE
			}
			CString stPerNum;
			int i = strArray.GetCount();
			stPerNum.Format(L"人数: %d",i);

			pWnd->SetDlgItemText(IDC_STATIC_PASSANGERNUM,stPerNum);
			pWnd->UpdateList();
		}
#ifdef RFID_TEST
		Sleep(5000);
#else
		Sleep(3); //加延时
#endif
	}
	return 0;
}