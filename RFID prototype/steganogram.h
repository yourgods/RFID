#if !defined(STEGANOGRAM_H__)
#define STEGANOGRAM_H__
#include "config.h"

#define maxim 65537
#define fuyi 65536
#define ones 65535
#define gainfour 61440
#define gainthree 3840
#define gaintwo 240
#define gain 1008
#define gainone 15
#define round 8

#define Af 0.5   
#define af 0
#define acs  10  
#define rcs  28
#define bcs  8/3  
#define hcs  0.01  

double GetInt(double dbX);
CString LogisticAlgorithm();
CString GetChaosKey(CString strInput);   
unsigned int inv(unsigned int key);
void code(CString input,char output[28],unsigned int keybox[7][10]);
void uncode(CString miweninput,char output[28],unsigned int keybox[7][10]);
void key(CString cskey,unsigned int keybox[7][10]);
void de_key(unsigned int keybox[7][10],unsigned int dekeybox[7][10]);
unsigned int chengfa(unsigned int a,unsigned int b);

#endif