#include "stdafx.h"
#include <mysql.h>
#include "mysql_com.h"
#include "common.h"
#include "database.h"
#include "Resource.h"
#include "SchoolInfoInput.h"
#include "ClassInfoInput.h"
#include "TeacherInfoInput.h"
#include "ParentInfoInput.h"
#include "StudentInfoInput.h"
//#include "BusInfo.h"
//#include "DriverInfoInput.h"
//#include "RoadInfoInput.h"
//#include "StationInfoInput.h"
#include "message.h"

extern struct table Table[16] = 
{
	//学校信息
	{
		_T("school"), 
		_T("学校信息"),
		SchoolInfoCreateDlg,
		SchooInfoQueryRight,
		1, 
		10,
		{true, MYSQL_TYPE_STRING, _T("school_ID"), _T("学校编号"), 
		false, MYSQL_TYPE_VAR_STRING, _T("name"), _T("学校名称"), 
		false, MYSQL_TYPE_STRING, _T("manager"), _T("主管"),
		false, MYSQL_TYPE_STRING, _T("mobi_phone"), _T("学校电话"),
		false, MYSQL_TYPE_VAR_STRING, _T("fix_phone"), _T("传真"),
		false, MYSQL_TYPE_LONG, _T("student_number"), _T("学生数"),
		false, MYSQL_TYPE_LONG, _T("class_number"), _T("班级个数"),
		false, MYSQL_TYPE_LONG, _T("teacher_number"), _T("教师个数"),
		false, MYSQL_TYPE_LONG, _T("bus_number"), _T("车辆个数"),
		false, MYSQL_TYPE_LONG, _T("driver_number"), _T("司机个数"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("备注")
		}
	},
	//班级信息
	{
		_T("class"),
		_T("班级信息"),
		ClassInfoCreateDlg,
		ClassInfoQueryRight,
		1,
		2,
		{true, MYSQL_TYPE_STRING, _T("class_ID"), _T("班级编号"),
		false, MYSQL_TYPE_STRING, _T("name"), _T("班级名称"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("备注")
		}
	},
	//教师信息
	{
		_T("teacher"),
		_T("教师信息"),
		TeacherInfoCreateDlg,
		TeacherInfoQueryRight,
		1,
		7,
		{true, MYSQL_TYPE_STRING, _T("teacher_ID"), _T("教师编号"),
		false, MYSQL_TYPE_STRING, _T("name"), _T("姓名"),
		false, MYSQL_TYPE_TINY, _T("sex"), _T("性别"),
		false, MYSQL_TYPE_LONG, _T("age"), _T("年龄"),
		false, MYSQL_TYPE_STRING, _T("mobi_phone"), _T("移动电话"),
		false, MYSQL_TYPE_VAR_STRING, _T("home_phone"), _T("家庭电话"),
		false, MYSQL_TYPE_VAR_STRING, _T("address"), _T("家庭地址"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("备注")
		}
	},
	//家长信息
	{
		 _T("parent"),
		 _T("家长信息"),
		 ParentInfoCreateDlg,
		 ParentInfoQueryRight,
		 1,
		 6,
		 {true, MYSQL_TYPE_STRING, _T("parent_ID"), _T("家长编号"),
		 false, MYSQL_TYPE_STRING, _T("name"), _T("姓名"),
		 false, MYSQL_TYPE_TINY, _T("sex"), _T("性别"),
		 false, MYSQL_TYPE_STRING, _T("mobi_phone"), _T("移动电话"),
		 false, MYSQL_TYPE_VAR_STRING, _T("home_phone"), _T("家庭电话"),
		 false, MYSQL_TYPE_VAR_STRING, _T("address"), _T("家庭住址"),
		 false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("备注")
		 }
	},
	//学生信息
	{
		 _T("student"),
		 _T("学生信息"),
		 StudentInfoCreateDlg,
		 StudentInfoQueryRight,
		 1,
		 6,
		 {true, MYSQL_TYPE_STRING, _T("student_ID"), _T("学生编号"),
		 false, MYSQL_TYPE_STRING, _T("name"), _T("学生姓名"),
		 false, MYSQL_TYPE_TINY, _T("sex"), _T("性别"),
		 false, MYSQL_TYPE_LONG, _T("age"), _T("年龄"),
		 false, MYSQL_TYPE_TIME, _T("enroll_time"), _T("入学时间"),
		 false, MYSQL_TYPE_VAR_STRING, _T("address"), _T("家庭住址"),
		 false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("备注")
		 }
	},
	//校车信息
	{
		_T("bus"),
		_T("校车信息"),
		NULL,
		NULL,
		//BusInfoCreateDlg,
		//BusInfoQueryRight,
		1, 
		5,
		{true, MYSQL_TYPE_STRING, _T("bus_ID"), _T("车牌号"),
		false, MYSQL_TYPE_STRING, _T("type"), _T("校车类型"),
		false, MYSQL_TYPE_SHORT, _T("bus_no"), _T("校车编号"),
		false, MYSQL_TYPE_SHORT, _T("capacity"), _T("校车载员容量"),
		false, MYSQL_TYPE_TIME, _T("buy_time"), _T("购车时间"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("备注")
		}
	},
	//司机信息
	{
		_T("driver"),
		_T("司机信息"),
		NULL,
		NULL,
		//DriverInfoCreateDlg,
		//DriverInfoQueryRight,
		1,
		7,
		{true, MYSQL_TYPE_STRING, _T("driver_ID"), _T("司机编号"),
		false, MYSQL_TYPE_STRING, _T("name"), _T("司机姓名"),
		false, MYSQL_TYPE_TINY, _T("sex"), _T("性别"),
		false, MYSQL_TYPE_LONG, _T("age"), _T("年龄"),
		false, MYSQL_TYPE_STRING, _T("mobi_phone"), _T("移动电话"),
		false, MYSQL_TYPE_VAR_STRING, _T("home_phone"), _T("家庭电话"),
		false, MYSQL_TYPE_VAR_STRING, _T("address"), _T("家庭地址"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("备注")
		}
	},
	//校车线路信息
	{
		_T("road"),
		_T("校车线路信息"),
		NULL,
		NULL,
		//RoadInfoCreateDlg,
		//RoadInfoQueryRight,
		1,
		4,
		{true, MYSQL_TYPE_STRING, _T("road_ID"), _T("线路编号"),
		false, MYSQL_TYPE_STRING, _T("name"), _T("线路名称"),
		false, MYSQL_TYPE_STRING, _T("start"), _T("起始站"),
		false, MYSQL_TYPE_STRING, _T("end"), _T("终点站"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("备注")
		}
	},
	//站点信息
	{
		_T("station"),
		_T("站点信息"),
		NULL,
		NULL,
		//StationInfoCreateDlg,
		//StationInfoQueryRight,
		1,
		2,
		{true, MYSQL_TYPE_STRING, _T("station_ID"), _T("站点编号"),
		false, MYSQL_TYPE_STRING, _T("name"), _T("站点名称"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("备注")
		}
	},
	//发车次信息
	{
		_T("bus_depart"),
		_T("发车次信息"),
		NULL,
		NULL,
		//BusDepartInfoQueryRight,
		1,
		6,
		{true, MYSQL_TYPE_STRING, _T("bus_run_ID"), _T("校车运行编号"),
		false, MYSQL_TYPE_STRING, _T("bus_ID"), _T("校车编号"),
		false, MYSQL_TYPE_STRING, _T("driver_ID"), _T("司机编号"),
		false, MYSQL_TYPE_STRING, _T("teacher_ID"), _T("随车教师编号"),
		false, MYSQL_TYPE_STRING, _T("road_ID"), _T("线路编号"),
		false, MYSQL_TYPE_SHORT, _T("student_number"), _T("发车学生数"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("备注")
		}
	},
	//校车运行信息
	{
		_T("bus_run"),
		_T("校车运行信息"),
		NULL,
		NULL,
		//BusRunInfoQueryRight,
		1,
		8,
		{true, MYSQL_TYPE_LONG, _T("ID"), _T("条目编号"),
		false, MYSQL_TYPE_STRING, _T("bus_run_ID"), _T("校车运行编号"),
		false, MYSQL_TYPE_SHORT, _T("curr_stu_num"), _T("当前学生数"),
		false, MYSQL_TYPE_DOUBLE, _T("longitude"), _T("经度"),
		false, MYSQL_TYPE_DOUBLE, _T("latitude"), _T("纬度"),
		false, MYSQL_TYPE_TIME, _T("curr_time"), _T("当前时间"),
		false, MYSQL_TYPE_FLOAT, _T("curr_speed"), _T("当前车速"),
		false, MYSQL_TYPE_STRING, _T("curr_station_ID"), _T("当前站点编号"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("备注")
		}
	},
	//校车学生信息
	{
		_T("bus_stu"),
		_T("校车学生信息"),
		NULL,
		NULL,
		//BusStudentInfoQueryRight,
		2,
		5,
		{true, MYSQL_TYPE_STRING, _T("bus_run_ID"), _T("校车运行编号"),
		true, MYSQL_TYPE_STRING, _T("student_ID"), _T("学生编号"),
		false, MYSQL_TYPE_STRING, _T("up_station_ID"), _T("上车站牌编号"),
		false, MYSQL_TYPE_TIME, _T("up_time"), _T("上车时间"),
		false, MYSQL_TYPE_STRING, _T("down_station_ID"), _T("下车站牌编号"),
		false, MYSQL_TYPE_TIME, _T("down_time"), _T("下车时间"),
		false, MYSQL_TYPE_STRING, _T("remark"), _T("备注")
		}
	},
	//学校班级关系表格
	{
		_T("school_class"),
		_T("学校班级关系"),
		NULL,
		NULL,
		1,
		2,
		{true, MYSQL_TYPE_STRING, _T("class_ID"), _T("班级编号"),
		false, MYSQL_TYPE_STRING, _T("school_ID"), _T("学校编号"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("备注")
		}
	},
	//学生家长关系表格
	{
		_T("stu_par"),
		_T("学生家长关系"),
		NULL,
		NULL,
		1,
		3,
		{true, MYSQL_TYPE_STRING, _T("student_ID"), _T("学生编号"),
		false, MYSQL_TYPE_STRING, _T("parent_ID"), _T("家长编号"),
		false, MYSQL_TYPE_STRING, _T("relation"), _T("学生家长关系"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("备注")
		}
	},
	//学生教师班级关系表格
	{
		_T("stu_tea_cla"),
		_T("学生教师班级关系"),
		NULL,
		NULL,
		1,
		3,
		{true, MYSQL_TYPE_STRING, _T("student_ID"), _T("学生编号"),
		false, MYSQL_TYPE_STRING, _T("class_ID"), _T("班级编号"),
		false, MYSQL_TYPE_STRING, _T("teacher_ID"), _T("教师编号"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("备注")
		}
	},
	//线路站点关系信息
	{
		_T("road_station"),
		_T("线路站点关系"),
		NULL,
		NULL,
		1,
		2,
		{true, MYSQL_TYPE_STRING, _T("road_ID"), _T("线路编号"),
		false, MYSQL_TYPE_STRING, _T("station_ID"), _T("站点编号"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("备注")
		}
	}
};