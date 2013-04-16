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
	//ѧУ��Ϣ
	{
		_T("school"), 
		_T("ѧУ��Ϣ"),
		SchoolInfoCreateDlg,
		SchooInfoQueryRight,
		1, 
		10,
		{true, MYSQL_TYPE_STRING, _T("school_ID"), _T("ѧУ���"), 
		false, MYSQL_TYPE_VAR_STRING, _T("name"), _T("ѧУ����"), 
		false, MYSQL_TYPE_STRING, _T("manager"), _T("����"),
		false, MYSQL_TYPE_STRING, _T("mobi_phone"), _T("ѧУ�绰"),
		false, MYSQL_TYPE_VAR_STRING, _T("fix_phone"), _T("����"),
		false, MYSQL_TYPE_LONG, _T("student_number"), _T("ѧ����"),
		false, MYSQL_TYPE_LONG, _T("class_number"), _T("�༶����"),
		false, MYSQL_TYPE_LONG, _T("teacher_number"), _T("��ʦ����"),
		false, MYSQL_TYPE_LONG, _T("bus_number"), _T("��������"),
		false, MYSQL_TYPE_LONG, _T("driver_number"), _T("˾������"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("��ע")
		}
	},
	//�༶��Ϣ
	{
		_T("class"),
		_T("�༶��Ϣ"),
		ClassInfoCreateDlg,
		ClassInfoQueryRight,
		1,
		2,
		{true, MYSQL_TYPE_STRING, _T("class_ID"), _T("�༶���"),
		false, MYSQL_TYPE_STRING, _T("name"), _T("�༶����"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("��ע")
		}
	},
	//��ʦ��Ϣ
	{
		_T("teacher"),
		_T("��ʦ��Ϣ"),
		TeacherInfoCreateDlg,
		TeacherInfoQueryRight,
		1,
		7,
		{true, MYSQL_TYPE_STRING, _T("teacher_ID"), _T("��ʦ���"),
		false, MYSQL_TYPE_STRING, _T("name"), _T("����"),
		false, MYSQL_TYPE_TINY, _T("sex"), _T("�Ա�"),
		false, MYSQL_TYPE_LONG, _T("age"), _T("����"),
		false, MYSQL_TYPE_STRING, _T("mobi_phone"), _T("�ƶ��绰"),
		false, MYSQL_TYPE_VAR_STRING, _T("home_phone"), _T("��ͥ�绰"),
		false, MYSQL_TYPE_VAR_STRING, _T("address"), _T("��ͥ��ַ"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("��ע")
		}
	},
	//�ҳ���Ϣ
	{
		 _T("parent"),
		 _T("�ҳ���Ϣ"),
		 ParentInfoCreateDlg,
		 ParentInfoQueryRight,
		 1,
		 6,
		 {true, MYSQL_TYPE_STRING, _T("parent_ID"), _T("�ҳ����"),
		 false, MYSQL_TYPE_STRING, _T("name"), _T("����"),
		 false, MYSQL_TYPE_TINY, _T("sex"), _T("�Ա�"),
		 false, MYSQL_TYPE_STRING, _T("mobi_phone"), _T("�ƶ��绰"),
		 false, MYSQL_TYPE_VAR_STRING, _T("home_phone"), _T("��ͥ�绰"),
		 false, MYSQL_TYPE_VAR_STRING, _T("address"), _T("��ͥסַ"),
		 false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("��ע")
		 }
	},
	//ѧ����Ϣ
	{
		 _T("student"),
		 _T("ѧ����Ϣ"),
		 StudentInfoCreateDlg,
		 StudentInfoQueryRight,
		 1,
		 6,
		 {true, MYSQL_TYPE_STRING, _T("student_ID"), _T("ѧ�����"),
		 false, MYSQL_TYPE_STRING, _T("name"), _T("ѧ������"),
		 false, MYSQL_TYPE_TINY, _T("sex"), _T("�Ա�"),
		 false, MYSQL_TYPE_LONG, _T("age"), _T("����"),
		 false, MYSQL_TYPE_TIME, _T("enroll_time"), _T("��ѧʱ��"),
		 false, MYSQL_TYPE_VAR_STRING, _T("address"), _T("��ͥסַ"),
		 false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("��ע")
		 }
	},
	//У����Ϣ
	{
		_T("bus"),
		_T("У����Ϣ"),
		NULL,
		NULL,
		//BusInfoCreateDlg,
		//BusInfoQueryRight,
		1, 
		5,
		{true, MYSQL_TYPE_STRING, _T("bus_ID"), _T("���ƺ�"),
		false, MYSQL_TYPE_STRING, _T("type"), _T("У������"),
		false, MYSQL_TYPE_SHORT, _T("bus_no"), _T("У�����"),
		false, MYSQL_TYPE_SHORT, _T("capacity"), _T("У����Ա����"),
		false, MYSQL_TYPE_TIME, _T("buy_time"), _T("����ʱ��"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("��ע")
		}
	},
	//˾����Ϣ
	{
		_T("driver"),
		_T("˾����Ϣ"),
		NULL,
		NULL,
		//DriverInfoCreateDlg,
		//DriverInfoQueryRight,
		1,
		7,
		{true, MYSQL_TYPE_STRING, _T("driver_ID"), _T("˾�����"),
		false, MYSQL_TYPE_STRING, _T("name"), _T("˾������"),
		false, MYSQL_TYPE_TINY, _T("sex"), _T("�Ա�"),
		false, MYSQL_TYPE_LONG, _T("age"), _T("����"),
		false, MYSQL_TYPE_STRING, _T("mobi_phone"), _T("�ƶ��绰"),
		false, MYSQL_TYPE_VAR_STRING, _T("home_phone"), _T("��ͥ�绰"),
		false, MYSQL_TYPE_VAR_STRING, _T("address"), _T("��ͥ��ַ"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("��ע")
		}
	},
	//У����·��Ϣ
	{
		_T("road"),
		_T("У����·��Ϣ"),
		NULL,
		NULL,
		//RoadInfoCreateDlg,
		//RoadInfoQueryRight,
		1,
		4,
		{true, MYSQL_TYPE_STRING, _T("road_ID"), _T("��·���"),
		false, MYSQL_TYPE_STRING, _T("name"), _T("��·����"),
		false, MYSQL_TYPE_STRING, _T("start"), _T("��ʼվ"),
		false, MYSQL_TYPE_STRING, _T("end"), _T("�յ�վ"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("��ע")
		}
	},
	//վ����Ϣ
	{
		_T("station"),
		_T("վ����Ϣ"),
		NULL,
		NULL,
		//StationInfoCreateDlg,
		//StationInfoQueryRight,
		1,
		2,
		{true, MYSQL_TYPE_STRING, _T("station_ID"), _T("վ����"),
		false, MYSQL_TYPE_STRING, _T("name"), _T("վ������"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("��ע")
		}
	},
	//��������Ϣ
	{
		_T("bus_depart"),
		_T("��������Ϣ"),
		NULL,
		NULL,
		//BusDepartInfoQueryRight,
		1,
		6,
		{true, MYSQL_TYPE_STRING, _T("bus_run_ID"), _T("У�����б��"),
		false, MYSQL_TYPE_STRING, _T("bus_ID"), _T("У�����"),
		false, MYSQL_TYPE_STRING, _T("driver_ID"), _T("˾�����"),
		false, MYSQL_TYPE_STRING, _T("teacher_ID"), _T("�泵��ʦ���"),
		false, MYSQL_TYPE_STRING, _T("road_ID"), _T("��·���"),
		false, MYSQL_TYPE_SHORT, _T("student_number"), _T("����ѧ����"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("��ע")
		}
	},
	//У��������Ϣ
	{
		_T("bus_run"),
		_T("У��������Ϣ"),
		NULL,
		NULL,
		//BusRunInfoQueryRight,
		1,
		8,
		{true, MYSQL_TYPE_LONG, _T("ID"), _T("��Ŀ���"),
		false, MYSQL_TYPE_STRING, _T("bus_run_ID"), _T("У�����б��"),
		false, MYSQL_TYPE_SHORT, _T("curr_stu_num"), _T("��ǰѧ����"),
		false, MYSQL_TYPE_DOUBLE, _T("longitude"), _T("����"),
		false, MYSQL_TYPE_DOUBLE, _T("latitude"), _T("γ��"),
		false, MYSQL_TYPE_TIME, _T("curr_time"), _T("��ǰʱ��"),
		false, MYSQL_TYPE_FLOAT, _T("curr_speed"), _T("��ǰ����"),
		false, MYSQL_TYPE_STRING, _T("curr_station_ID"), _T("��ǰվ����"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("��ע")
		}
	},
	//У��ѧ����Ϣ
	{
		_T("bus_stu"),
		_T("У��ѧ����Ϣ"),
		NULL,
		NULL,
		//BusStudentInfoQueryRight,
		2,
		5,
		{true, MYSQL_TYPE_STRING, _T("bus_run_ID"), _T("У�����б��"),
		true, MYSQL_TYPE_STRING, _T("student_ID"), _T("ѧ�����"),
		false, MYSQL_TYPE_STRING, _T("up_station_ID"), _T("�ϳ�վ�Ʊ��"),
		false, MYSQL_TYPE_TIME, _T("up_time"), _T("�ϳ�ʱ��"),
		false, MYSQL_TYPE_STRING, _T("down_station_ID"), _T("�³�վ�Ʊ��"),
		false, MYSQL_TYPE_TIME, _T("down_time"), _T("�³�ʱ��"),
		false, MYSQL_TYPE_STRING, _T("remark"), _T("��ע")
		}
	},
	//ѧУ�༶��ϵ���
	{
		_T("school_class"),
		_T("ѧУ�༶��ϵ"),
		NULL,
		NULL,
		1,
		2,
		{true, MYSQL_TYPE_STRING, _T("class_ID"), _T("�༶���"),
		false, MYSQL_TYPE_STRING, _T("school_ID"), _T("ѧУ���"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("��ע")
		}
	},
	//ѧ���ҳ���ϵ���
	{
		_T("stu_par"),
		_T("ѧ���ҳ���ϵ"),
		NULL,
		NULL,
		1,
		3,
		{true, MYSQL_TYPE_STRING, _T("student_ID"), _T("ѧ�����"),
		false, MYSQL_TYPE_STRING, _T("parent_ID"), _T("�ҳ����"),
		false, MYSQL_TYPE_STRING, _T("relation"), _T("ѧ���ҳ���ϵ"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("��ע")
		}
	},
	//ѧ����ʦ�༶��ϵ���
	{
		_T("stu_tea_cla"),
		_T("ѧ����ʦ�༶��ϵ"),
		NULL,
		NULL,
		1,
		3,
		{true, MYSQL_TYPE_STRING, _T("student_ID"), _T("ѧ�����"),
		false, MYSQL_TYPE_STRING, _T("class_ID"), _T("�༶���"),
		false, MYSQL_TYPE_STRING, _T("teacher_ID"), _T("��ʦ���"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("��ע")
		}
	},
	//��·վ���ϵ��Ϣ
	{
		_T("road_station"),
		_T("��·վ���ϵ"),
		NULL,
		NULL,
		1,
		2,
		{true, MYSQL_TYPE_STRING, _T("road_ID"), _T("��·���"),
		false, MYSQL_TYPE_STRING, _T("station_ID"), _T("վ����"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("��ע")
		}
	}
};