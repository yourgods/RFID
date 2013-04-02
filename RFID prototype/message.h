#if !defined(MESSAGE_H__)
#define MESSAGE_H__

#include "config.h"
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

#endif