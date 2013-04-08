#if !defined(MESSAGE_H__)
#define MESSAGE_H__

#include "config.h"

#if 0
//index 1
struct bus_depart{
	char bus_run_ID[30];
	char bus_ID[20];
	char driver_ID[20];
	char teacher_ID[20];
	char road_ID[20];
	char student_number[20];
	char remark[50];
};

//index 2
struct bus_run{
	char bus_run_ID[30];
	char curr_stu_num[20];
	char longitude[20];
	char latitude[20];
	char curr_time[20];
	char curr_speed[20];
	char curr_station_ID[20];
	char remark[50];
};

//index 3
struct bus_stu{
	char bus_run_ID[20];
	char student_ID[20];
	char up_station_ID[20];
	char up_time[20];
	char down_station_ID[20];
	char down_time[20];
	char remark[50];
};
#else

//index 1
struct bus_depart{
	CString bus_run_ID;
	CString bus_ID;
	CString driver_ID;
	CString teacher_ID;
	CString road_ID;
	CString student_number;
	CString remark;
};

//index 2
struct bus_run{
	CString bus_run_ID;
	CString curr_stu_num;
	CString longitude;
	CString latitude;
	CString curr_time;
	CString curr_speed;
	CString curr_station_ID;
	CString remark;
};

//index 3
struct bus_stu{
	CString bus_run_ID;
	CString student_ID;
	CString up_station_ID;
	CString up_time;
	CString down_station_ID;
	CString down_time;
	CString remark;
};
#endif

#endif