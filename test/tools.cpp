#include "stdafx.h"
#include "tools.h"

wchar_t *AnsiToUnicode(char *szAnsi)
{
	//Ԥת�����õ�����ռ�Ĵ�С
    int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), NULL, 0);

    //����ռ�Ҫ��'\0'�����ռ䣬MultiByteToWideChar�����'\0'�ռ�
    wchar_t* wszString = new wchar_t[wcsLen + 1];

    //ת��
    ::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), wszString, wcsLen);

    //������'\0'
    wszString[wcsLen] = '\0';
	return wszString;
}

char *UnicodeToAnsi(wchar_t *wszString)
{
	//Ԥת�����õ�����ռ�Ĵ�С������õĺ��������������෴
    int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);

    //ͬ�ϣ�����ռ�Ҫ��'\0'�����ռ�
    char* szAnsi = new char[ansiLen + 1];

    //ת��
    //unicode���Ӧ��strlen��wcslen
    ::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), szAnsi, ansiLen, NULL, NULL);

    //������'\0'
    szAnsi[ansiLen] = '\0';
	return szAnsi;
}

char *UnicodeToUTF8(wchar_t *wszString)
{
	//Ԥת�����õ�����ռ�Ĵ�С������õĺ��������������෴
    int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);

    //ͬ�ϣ�����ռ�Ҫ��'\0'�����ռ�
    //UTF8��Ȼ��Unicode��ѹ����ʽ����Ҳ�Ƕ��ֽ��ַ��������Կ�����char����ʽ����
    char* szU8 = new char[u8Len + 1];

    //ת��
    //unicode���Ӧ��strlen��wcslen
    ::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), szU8, u8Len, NULL, NULL);

    //������'\0'
    szU8[u8Len] = '\0';
	
	return szU8;
}

wchar_t *UTF8ToUnicode(char *szU8)
{
    //Ԥת�����õ�����ռ�Ĵ�С
    int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);

    //����ռ�Ҫ��'\0'�����ռ䣬MultiByteToWideChar�����'\0'�ռ�
    wchar_t* wszString = new wchar_t[wcsLen + 1];

    //ת��
    ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);

    //������'\0'
    wszString[wcsLen] = '\0';

	return wszString;
}