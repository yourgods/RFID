/*********************************************************************************************
�ļ����ƣ�        WqLib_2416.h
������            Copyright (c) ����΢Ƕ������Ƽ����޹�˾.  All rights reserved.
�����Լ�˵����    ΢Ƕ������� ��̬���� �м��
�汾�ţ�          V1.00
���ߣ�
�޸��˼����ڣ�    ����΢Ƕ 2011-7-04
�޸ļ�¼��        
***********************************************************************************************/
#ifndef WQLIB_2416_H__
#define WQLIB_2416_H__

#define DLLEXPORT	__declspec(dllexport)
#define EXTERNC		extern "C"

/*********************************************************************************************
�������ƣ�RebootHot
��    �ܣ������豸(������,������)
�����������
�����������
��    �أ���
***********************************************************************************************/
EXTERNC DLLEXPORT void RebootHot(void);

/*********************************************************************************************
�������ƣ�RE_BOOT(������,������)
��    �ܣ������豸
�����������
�����������
��    �أ���
***********************************************************************************************/
EXTERNC DLLEXPORT void RE_BOOT(void);  


/*********************************************************************************************
�������ƣ� Turn_Light
��    �ܣ� �ر���Ϳ�����(���߹ر�VGA������VGA���)
��������� on_off=0�ر���;  on_off=1 ������
��������� ��
��    �أ� ��
***********************************************************************************************/
EXTERNC DLLEXPORT void Turn_Light(int on_off);


/*********************************************************************************************
�������ƣ� Turn_Beep ���Ƽ�ʹ�ã�Ӧ��ʹ��BeepOnce
��    �ܣ� �ط������Ϳ�������(�󲿷ְ汾��Turn_Beep(1), ����ʱ,Turn_Beep(0),��һ��)��
��������� on_off=0�ط�����;  on_off=1 ���������� 
��������� ��
��    �أ� ��
***********************************************************************************************/
EXTERNC DLLEXPORT void Turn_Beep(int on_off);


/*********************************************************************************************
�������ƣ� BeepOnce
��    �ܣ� ���ú󣬷�������һ��(ע������������ã���������ʱSleep(120)��120ms)
��������� bVer��0�汾1��1�汾2���汾1��2���Ӳ�������1���죬��ʹ�ð汾2���°���Զ�ʶ��bVer��Ϊ���⣬
��������� ��
��    �أ� ��
***********************************************************************************************/
EXTERNC DLLEXPORT void BeepOnce(BOOL bVer);

/*********************************************************************************************
�������ƣ� Beep
˵    ���� ע�⣺ĳЩ�ͺſ��ܲ�֧�֣�5.6�硢8���S3C2416������Բ�֧�ָú�����
��    �ܣ� ����PC��Beep�������Բ�ͬ��Ƶ�ʣ���ͬʱ�����С���Beep(200, 1000); //����1s�ı�����
��������� dwFrequency: Ƶ�ʣ�3 ~ 20 000Hz��
           dwSoundDelay��ʹ�������Ƶ�����е�ʱ�䣬msΪ��λ
��������� ��
��    �أ� ��
***********************************************************************************************/
EXTERNC DLLEXPORT void Beep(DWORD dwFrequency, DWORD dwSoundDelay);

/*********************************************************************************************
�������ƣ� SetMemoryDiv
��    �ܣ� ����洢�ڴ�(RAM�ļ�ϵͳռ��)�ͳ����ڴ�ı���
           �����洢�ڴ�Ϊ5M�ֽڣ�SetMemoryDiv(0x500000/0x1000);0x1000��ҳ�Ĵ�С
��������� dwStorePages: �洢�ڴ��С����pageΪ��λ��
��������� ��
��    �أ� TRUE or FALSE
***********************************************************************************************/
EXTERNC DLLEXPORT int SetMemoryDiv(DWORD dwStorePages);


/*********************************************************************************************
�������ƣ� GetMemoryDiv
��    �ܣ� ��ȡ�洢�ڴ�ͳ����ڴ�ı�������ҳ�Ĵ�С��
��������� ��
��������� lpdwStorePages������洢�ڴ��С��ҳΪ��λ
           lpdwRamPages:   ��������ڴ��С��ҳΪ��λ
           lpdwPageSize:   ҳ��С
��    �أ� TRUE or FALSE
***********************************************************************************************/
EXTERNC DLLEXPORT int GetMemoryDiv(LPDWORD lpdwStorePages, LPDWORD lpdwRamPages, LPDWORD lpdwPageSize);



/*********************************************************************************************
�������ƣ� SetUartFifo
��    �ܣ� ���ø������ڽ���FIFO�жϴ����ֽ�����1�ֽڴ���ʱ������ʵʱ���մ��ڵ�ÿ���ֽڡ�
��������� dwSize: 0:1�ֽڴ�����1:8�ֽڣ�2:16�ֽڣ�3:32�ֽڣ�����ֵ����
           dwCOM��1��ʱ���ӦCOM1��2��ӦCOM2��3 COM3��4 COM4������ֵ����
��������� ��
ע    �⣺ �򿪴��ں��øú����������á�������ʱ��Ĭ����8�ֽڴ�����һ�㲻�������á�
��    �أ� TRUE or FALSE��FALSEʱ����Ҫ�ǲ���������
***********************************************************************************************/
EXTERNC DLLEXPORT BOOL SetUartFifo(DWORD dwSize, DWORD dwCOM);

#endif