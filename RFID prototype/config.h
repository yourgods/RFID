#if !defined(CONFIG_H__)
#define CONFIG_H__

#define DEBUG_YFH
#define USE_TRAFFIC

//ini文件路径及名称
#define INI_PATH _T("\\ResidentFlash\\Config\\")
#define INT_NAME _T("config")
#define DRIVER_ID _T("Driver_01")
#define TEACHER_ID _T("Teacher_01")
#define GSM_COM _T("COM1:")
#define RFID_COM _T("COM2:")
#define GPS_COM _T("COM3:")
#define OTHERS_COM _T("COM4:")
#define SMSC _T("8613010305500")  //for anhui
#define ELAPSE (60*1000) //s
#define TIMER 1
#define TIMER2 2
#define ELAPSE2 (1*1000) //s

#endif