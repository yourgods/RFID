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
	CString name;  //���ݿ����
	CString chineseName; //��Ӧ��������
	int itemKeyCount; //��������
	int itemOthersCount; //�����и���
	struct fieldAttr fieldValue[15]; //���е�����
};
#endif

extern struct table Table[9] = 
{
	//ѧУ��Ϣ
	{
		_T("school"), 
		_T("ѧУ��Ϣ"),
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
		1,
		2,
		{true, MYSQL_TYPE_STRING, _T("class_ID"), _T("�༶���"),
		false, MYSQL_TYPE_STRING, _T("name"), _T("�༶����"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("��ע")
		}
	},
	//ѧ����Ϣ
	{
		 _T("student"),
		 _T("ѧ����Ϣ"),
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
	//��ʦ��Ϣ
	{
		_T("teacher"),
		_T("��ʦ��Ϣ"),
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
	//У����Ϣ
	{
		_T("bus"),
		_T("У����Ϣ"),
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
		1,
		4,
		{true, MYSQL_TYPE_STRING, _T("road_ID"), _T("��·���"),
		false, MYSQL_TYPE_STRING, _T("name"), _T("��·����"),
		false, MYSQL_TYPE_STRING, _T("start"), _T("��ʼվ��"),
		false, MYSQL_TYPE_STRING, _T("end"), _T("�յ�վ"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("��ע")
		}
	},
	//վ����Ϣ
	{
		_T("station"),
		_T("վ����Ϣ"),
		1,
		2,
		{true, MYSQL_TYPE_STRING, _T("station_ID"), _T("վ����"),
		false, MYSQL_TYPE_STRING, _T("name"), _T("վ������"),
		false, MYSQL_TYPE_VAR_STRING, _T("remark"), _T("��ע")
		}
	}
};

//ѧУ��Ϣ����
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

