// SmsTraffic.h: interface for the CSmsTraffic class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(SMSTRAFFIC_H__)
#define SMSTRAFFIC_H__

#include "Sms.h"
#include "Comm.h"
#include "config.h"

#define MAX_SM_SEND		128		// ���Ͷ��г���
#define MAX_SM_RECV		128		// ���ն��г���

class CSmsTraffic  
{
public:
	CSmsTraffic();
	virtual ~CSmsTraffic();

	int m_nSendIn;		// ���Ͷ��е�����ָ��
	int m_nSendOut;		// ���Ͷ��е����ָ��

	int m_nRecvIn;		// ���ն��е�����ָ��
	int m_nRecvOut;		// ���ն��е����ָ��

	SM_PARAM m_SmSend[MAX_SM_SEND];		// ���Ͷ���Ϣ����
	SM_PARAM m_SmRecv[MAX_SM_SEND];		// ���ն���Ϣ����

	CRITICAL_SECTION m_csSend;		// �뷢����ص��ٽ��
	CRITICAL_SECTION m_csRecv;		// �������ص��ٽ��

	HANDLE m_hKillThreadEvent;		// ֪ͨ���̹߳رյ��¼�
	HANDLE m_hThreadKilledEvent;	// ���߳�����رյ��¼�

	void PutSendMessage(SM_PARAM* pSmParam);	// ������Ϣ���뷢�Ͷ���
	BOOL GetSendMessage(SM_PARAM* pSmParam);	// �ӷ��Ͷ�����ȡһ������Ϣ
	void PutRecvMessage(SM_PARAM* pSmParam, int nCount);	// ������Ϣ������ն���
	BOOL GetRecvMessage(SM_PARAM* pSmParam);	// �ӽ��ն�����ȡһ������Ϣ

	static UINT SmThread(LPVOID lpParam);	// ����Ϣ�շ��������߳�
};

#endif