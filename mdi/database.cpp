#include "stdafx.h"
#include <mysql.h>
#include "mysql_com.h"
#include "common.h"
#include "database.h"
#if 0
struct fieldAttr
{
	bool primaryKey;
	enum enum_field_types fieldType;
	CString fieldName;
	CString chineseName;
};

struct table
{
	CString name;  //数据库表名
	CString chineseName; //对应中文名称
	int itemKeyCount; //主键个数
	int itemOthersCount; //其他列个数
	struct fieldAttr fieldValue[15]; //各列的属性
};
#endif

extern struct table Table[9] = 
{
	//学校信息
	{
		_T("school"), 
		_T("学校信息"),
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
		1,
		2,
		{true, MYSQL_TYPE_STRING, _T("class_ID"), _T("班级编号"),
		false, MYSQL_TYPE_STRING, _T("name"), _T("班级名称"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("备注")
		}
	},
	//学生信息
	{
		 _T("student"),
		 _T("学生信息"),
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
	//教师信息
	{
		_T("teacher"),
		_T("教师信息"),
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
	//校车信息
	{
		_T("bus"),
		_T("校车信息"),
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
		1,
		4,
		{true, MYSQL_TYPE_STRING, _T("road_ID"), _T("线路编号"),
		false, MYSQL_TYPE_STRING, _T("name"), _T("线路名称"),
		false, MYSQL_TYPE_STRING, _T("start"), _T("起始站点"),
		false, MYSQL_TYPE_STRING, _T("end"), _T("终点站"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("备注")
		}
	},
	//站点信息
	{
		_T("station"),
		_T("站点信息"),
		1,
		2,
		{true, MYSQL_TYPE_STRING, _T("station_ID"), _T("站点编号"),
		false, MYSQL_TYPE_STRING, _T("name"), _T("站点名称"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("备注")
		}
	}
};

//学校信息操作
int SchoolInfoQuery(CString school_ID, struct results **pRet, int *count)
{
	struct query query;
	query.name = _T("school");
	query.itemCount = 0;
	if(!school_ID.IsEmpty()){
		query.itemCount = 1;
		query.itemValue[0].name = _T("school_ID");
		query.itemValue[0].valueType =(enum enum_field_types)1 ;
		query.itemValue[0].value = school_ID;
	}
	return Query(&query, pRet, count);
}

int SchoolInfoInsertUniform(struct  update *)
{
	return TRUE;
}

int SchoolInfoInsert()
{
	return TRUE;
}

