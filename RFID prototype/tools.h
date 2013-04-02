#if !defined(TOOLS_H__)
#define TOOLS_H__
#include "config.h"

int Char2Hex(char ch);
char Hex2Char(unsigned int n);
long MIndex(int num, int index);
long String2Hex(char* string, int strlen);
int Hex2String(long hex, char* string, int* slen);
wchar_t Hex2Wchart(long hex);
int HexStr2WChars(char *hexstr, int hexstrlen, int eachchar, wchar_t *wchs, int *wchsLen);
long Wchart2Hex(wchar_t wch);
int Wchart2HexStr(wchar_t *wchs, int wchslen, int eachchar, char* hexstr, int* hexstrlen);
void GetCurrentDirectory(CString &szPath);
UINT convert_char(CString str1);
void UnicodeToGb2312(CString str1);
char *UnicodeToAnsi(wchar_t *);
wchar_t *AnsiToUnicode(char *);
char *UnicodeToUTF8(wchar_t *);
wchar_t *UTF8ToUnicode(char *);

#endif