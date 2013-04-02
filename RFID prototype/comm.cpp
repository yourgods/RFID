#include "stdafx.h"
#include "Comm.h"

// �����豸���
HANDLE hComm;

// �򿪴���
// ����: pPort - �������ƻ��豸·��������"COM1"��"\\.\COM1"���ַ�ʽ�������ú���
//       nBaudRate - ������
//       nParity - ��żУ��
//       nByteSize - �����ֽڿ��
//       nStopBits - ֹͣλ
BOOL OpenComm(const char* pPort, int nBaudRate, int nParity, int nByteSize, int nStopBits)
{
	DCB dcb;		// ���ڿ��ƿ�
	COMMTIMEOUTS timeouts = {	// ���ڳ�ʱ���Ʋ���
		100,				// ���ַ������ʱʱ��: 100 ms
		1,					// ������ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
		500,				// ������(�����)����ʱʱ��: 500 ms
		1,					// д����ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
		100};				// ������(�����)д��ʱʱ��: 100 ms

	CString port(pPort);
	hComm = CreateFile(port,	// �������ƻ��豸·��
			GENERIC_READ | GENERIC_WRITE,	// ��д��ʽ
			0,				// ����ʽ����ռ
			NULL,			// Ĭ�ϵİ�ȫ������
			OPEN_EXISTING,	// ������ʽ
			0,				// ���������ļ�����
			NULL);			// �������ģ���ļ�
	
	if(hComm == INVALID_HANDLE_VALUE) return FALSE;		// �򿪴���ʧ��



	GetCommState(hComm, &dcb);		// ȡDCB

	dcb.BaudRate = nBaudRate;
	dcb.ByteSize = nByteSize;
	dcb.Parity = nParity;
	dcb.StopBits = nStopBits;

	if(SetCommState(hComm, &dcb) == 0){
		AfxMessageBox(_T("Set comm rate fail"));	
	}		// ����DCB

	if(SetupComm(hComm, 4096, 4096) == 0){
		AfxMessageBox(_T("Setup comm fail"));
	}	// �������������������С

	if(SetCommTimeouts(hComm, &timeouts) == 0){
		AfxMessageBox(_T("SetCommTimeout"));
	}	// ���ó�ʱ

	return TRUE;
}

// �رմ���
BOOL CloseComm()
{
	return CloseHandle(hComm);
}

// д����
// ����: pData - ��д�����ݻ�����ָ��
//       nLength - ��д�����ݳ���
// ����: ʵ��д������ݳ���
int WriteComm(void* pData, int nLength)
{
	DWORD dwNumWrite;	// ���ڷ��������ݳ���

	char *p = (char *)pData;
	WriteFile(hComm, pData, (DWORD)nLength, &dwNumWrite, NULL);

	return (int)dwNumWrite;
}

// ������
// ����: pData - ���������ݻ�����ָ��
//       nLength - ������������ݳ���
// ����: ʵ�ʶ��������ݳ���
int ReadComm(void* pData, int nLength)
{
	DWORD dwNumRead;	// �����յ������ݳ���

	memset(pData, 0x00, nLength);
	ReadFile(hComm, pData, (DWORD)nLength, &dwNumRead, NULL);
	return (int)dwNumRead;
}
