#include "stdafx.h"
#include "tools.h"
#include "database.h"

int Char2Hex(char ch)
{
	int n = -1;
	switch(ch)
	{
	case '0':	n = 0;	break;
	case '1':	n = 1;	break;
	case '2':	n = 2;	break;
	case '3':	n = 3;	break;
	case '4':	n = 4;	break;
	case '5':	n = 5;	break;
	case '6':	n = 6;	break;
	case '7':	n = 7;	break;
	case '8':	n = 8;	break;
	case '9':	n = 9;	break;
	case 'A':
	case 'a':	n = 10;	break;
	case 'B':
	case 'b':	n = 11;	break;
	case 'C':
	case 'c':	n = 12;	break;
	case 'D':
	case 'd':	n = 13;	break;
	case 'E':
	case 'e':	n = 14;	break;
	case 'F':
	case 'f':	n = 15;	break;
	default:	break;
	}

	return n;
}

// hex to char
// error: return -1 
char Hex2Char(unsigned int n)
{
	char ch;
	if(n>=0 && n<=9)	ch = 48 + n;
	else if(n>=10 && n<=15)	ch = 65 - 10 + n;
	else ch = -1;

	return ch;
}

// num^index
long MIndex(int num, int index)
{
	long s = 1;
	int i=0;
	while(i<index)
	{
		s *= num;
		i++;
	}

	return s;
}

// string to hex code
// error: return -1 
long String2Hex(char* string, int strlen)
{
	long hex=-1;
	int i=0, n=0;
	char *p = string;
	p += strlen - 1;
	if(string == NULL)	return hex;
	if(strlen <= 0 || strlen > 10)	return hex;

	hex = 0;
	do
	{
		n = Char2Hex(*p--);
		hex += n*MIndex(16, i++);
	}while(i<strlen);

	return hex;
}

// hex to char*
// string==NULL,slen = the size of string(slen as output)
// string!=NULL,input the length of string
// error: return -1
int Hex2String(long hex, char* string, int* slen)
{
	char tmp[11] = {0};
	if(hex < 0)	return -1;
	if(string == NULL){// count the length it will be used
		sprintf(tmp, "%x", hex);
		*slen = strlen(tmp);
		return 1;
	}
	memset(string, 0, *slen);
	sprintf(string, "%x", hex);

	return 1;
}

// hex to wchar_t code
// eg: input 0x5e74, return 年
// error: return -1
wchar_t Hex2Wchart(long hex)
{
	wchar_t wch = -1;
	if(hex <0)	return wch;
	wch = (wchar_t)hex;

	return wch;
}

// hex string to wchar_t*
// UNICODE十六进制字符串转成中英文
// hexstr每eachchar转换为一个wchar_t
// wchs == NULL, wchsLen as output(the size of wchs will be used)
// error: return -1
int HexStr2WChars(char *hexstr, int hexstrlen, int eachchar, wchar_t *wchs, int *wchsLen)
{
	if(hexstr == NULL || hexstrlen <= 0 || eachchar <= 0)	return -1;
	if(wchs == NULL){// count the size wchs it will be used
		*wchsLen = hexstrlen/eachchar + (hexstrlen%eachchar>0 ? 1 : 0);
		return 1;
	}
	memset(wchs, 0, *wchsLen * sizeof(wchar_t));
	char* tmp = new char[eachchar+1];
	char* p = hexstr;
	wchar_t* pwch = wchs;
	for(int i=0; i<hexstrlen; i+=eachchar){
		memset(tmp, 0, eachchar+1);
		// get eachchar char
		for(int j=0; j<eachchar; j++){
			if(i+j > hexstrlen)	break;
			tmp[j] = *p++;
		}
		// char* to hex
		long hex = String2Hex(tmp, strlen(tmp));
		if(hex == -1)	continue;
		// hex to wchar_t
		*pwch++ = Hex2Wchart(hex);
	}

	if(tmp)	delete []tmp;

	return 1;
}

// wchar_t to hex code
long Wchart2Hex(wchar_t wch)
{
	return (long)wch;
}

// wchar_t* to char*(hex string)
// 中英文转成UNICODE十六进制字符串
// eachchar用于控制每个wchar_t转换成多少个char字符
// hexstr == NULL,hexstrlen as output(the size of hexstr will be used)
// error: return -1
int Wchart2HexStr(wchar_t *wchs, int wchslen, int eachchar, char* hexstr, int* hexstrlen)
{
	if(wchs == NULL || wchslen <= 0 || eachchar <= 0)	return -1;
	if(hexstr == NULL){// count the size of hexstr will be used
		*hexstrlen = wchslen*eachchar;
		return 1;
	}
	memset(hexstr, 0, *hexstrlen);
	char* p = hexstr;
	wchar_t* pwch = wchs;
	char* tmp = new char[eachchar+1];
	for(int i=0; i<wchslen; i++){
		// wchar_t to hex
		long hex = Wchart2Hex(*pwch++);
		// hex to char*
		int charlen = 0;
		if(Hex2String(hex, NULL, &charlen) != -1){
			char* str = new char[charlen+1];
			memset(str, 0, charlen+1);
			int n = Hex2String(hex, str, &charlen);
			if(n != -1){
				int k=0;
				memset(tmp, 0, eachchar+1);
				for(k=0; k<eachchar-charlen; k++)	tmp[k] = '0';
				tmp = strcat(tmp, str);
				p = strcat(p, tmp);
			}
			if(str)	delete []str;
		}
		if(i > *hexstrlen)	break;
	}
	if(tmp)	delete []tmp;

	return 1;
}

//获取程序当前路径
void GetCurrentDirectory(CString &szPath)
{
	wchar_t pBuf[256];
	GetModuleFileName(NULL,pBuf,sizeof(pBuf)/sizeof(wchar_t));
	szPath=pBuf;
	szPath = szPath.Left(szPath.ReverseFind('\\')+1);
}
//这两个函数都是将UNICODE码转换成Gb2312,这个的优势是转换速度快。
UINT convert_char(CString str1)
{
	char *temp1;
	temp1 = (char *)(char*)(LPCTSTR)str1;
	UINT i;
	for(i=0;i<1024;i++)
	{
		send_data[i] = *temp1++;
		if(*temp1 == 0)
			temp1++;
		if(send_data[i] == 0)
			break;
	}
	send_data[i] = 0;
	return i;
}
//这两个函数都是将UNICODE码转换成Gb2312,这个的优势是可以转换中文。
void UnicodeToGb2312(CString str1)
{
	WideCharToMultiByte(CP_ACP,0,(LPCWSTR)str1,-1,(LPSTR)send_data,sizeof(send_data),NULL,NULL);
}

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