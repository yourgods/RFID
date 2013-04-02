#include "stdafx.h"
#include "tools.h"

wchar_t *AnsiToUnicode(char *szAnsi)
{
	//预转换，得到所需空间的大小
    int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), NULL, 0);

    //分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
    wchar_t* wszString = new wchar_t[wcsLen + 1];

    //转换
    ::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), wszString, wcsLen);

    //最后加上'\0'
    wszString[wcsLen] = '\0';
	return wszString;
}

char *UnicodeToAnsi(wchar_t *wszString)
{
	//预转换，得到所需空间的大小，这次用的函数和上面名字相反
    int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);

    //同上，分配空间要给'\0'留个空间
    char* szAnsi = new char[ansiLen + 1];

    //转换
    //unicode版对应的strlen是wcslen
    ::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), szAnsi, ansiLen, NULL, NULL);

    //最后加上'\0'
    szAnsi[ansiLen] = '\0';
	return szAnsi;
}

char *UnicodeToUTF8(wchar_t *wszString)
{
	//预转换，得到所需空间的大小，这次用的函数和上面名字相反
    int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);

    //同上，分配空间要给'\0'留个空间
    //UTF8虽然是Unicode的压缩形式，但也是多字节字符串，所以可以以char的形式保存
    char* szU8 = new char[u8Len + 1];

    //转换
    //unicode版对应的strlen是wcslen
    ::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), szU8, u8Len, NULL, NULL);

    //最后加上'\0'
    szU8[u8Len] = '\0';
	
	return szU8;
}

wchar_t *UTF8ToUnicode(char *szU8)
{
    //预转换，得到所需空间的大小
    int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);

    //分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
    wchar_t* wszString = new wchar_t[wcsLen + 1];

    //转换
    ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);

    //最后加上'\0'
    wszString[wcsLen] = '\0';

	return wszString;
}