// Comm.h: interface for the CComm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(COMM_H__)
#define COMM_H__

#include "config.h"

BOOL OpenComm(CString port, int nBaudRate=9600, int nParity=NOPARITY, int nByteSize=8, int nStopBits=ONESTOPBIT);
BOOL CloseComm();
int ReadComm(void* pData, int nLength);
int WriteComm(void* pData, int nLength);

#endif // !defined(COMM_H__)
