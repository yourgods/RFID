// mdi.h : mdi Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CmdiApp:
// �йش����ʵ�֣������ mdi.cpp
//

class CmdiApp : public CWinApp
{
public:
	CmdiApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CmdiApp theApp;