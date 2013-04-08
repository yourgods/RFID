
drop table if exists bus;
create table bus (
	bus_ID char(20) not null primary key,
	type char(20),
	bus_no smallint(4),
	capacity smallint(4),
	buy_time datetime
	remark varchar(255));

drop table if exists bus_depart;
create table bus_depart (
	bus_run_ID char(30) not null primary key,
	bus_ID char(20),
	driver_ID char(20),
	teacher_ID char(20),
	road_ID char(20),
	student_number smallint(4),
	remark varchar(255));

drop table if exists bus_run;
create table bus_run (
	ID int unsigned not null primary key auto_increment,
	bus_run_ID char(30) not null,
	curr_stu_num smallint(4),
	longitude double,
	latitude double,
	curr_time datetime,
	curr_speed float,
	curr_station_id char(20),
	remark varchar(255));

drop table if exists bus_stu;
create table bus_stu (
	bus_run_ID char(30) not null,
	student_ID char(20) not null,
	up_station_ID char(20),
	up_time datetime,
	down_station_ID char(20),
	down_time datetime,
	remark varchar(255),
	primary key (bus_run_ID, student_ID));

drop table if exists class;
create table class (
	class_ID char(20) not null primary key,
	name char(20),
	remark varchar(255));

drop table if exists driver;
create table driver (
	driver_ID char(20) not null primary key,
	name char(20),
	sex tinyint(1),
	age int(8),
	mobi_phone char(12),
	home_phone varchar(12),
	address varchar(255),
	remark varchar(255));

drop table if exists parent;
create table parent (
	parent_ID char(20) not null primary key,
	name char(20),
	sex tinyint(1),
	mobi_phone char(12),
	home_phone varchar(12),
	address varchar(255),
	remark varchar(255));

drop table if exists parent_log;
create table parent_log (
	parent_log_ID char(20) not null primary key,
	password char(32) not null,
	parent_ID char(20) not null,
	remark varchar(255));

drop table if exists road;
create table road (
	road_ID char(20) not null primary key,
	name char(20),
	start char(20),
	end char(20),
	remark varchar(255));

drop table if exists road_station;
create table road_station (
	road_ID char(20) not null,
	station_ID char(20) not null,
	remark varchar(255),
	primary key (road_ID, station_ID));

drop table if exists school;
create table school (
	school_ID char(20) not null primary key,
	name varchar(50),
	manager char(20),
	mobi_phone char(12),
	fix_phone varchar(12),
	student_number int(8),
	class_number int(8),
	teacher_number int(8),
	bus_number int(8),
	driver_number int(8),
	remark varchar(255));

drop table if exists station;
create table station (
	station_ID char(20) not null primary key,
	name char(20),
	remark varchar(255));

drop table if exists stu_par;
create table stu_par (
	student_ID char(20) not null,
	parent_ID char(20) not null,
	relation char(20),
	remark varchar(255),
	primary key (student_ID, parent_ID));

drop table if exists stu_tea_cla;
create table stu_tea_cla (
	student_ID char(20) not null primary key,
	class_ID char(20),
	teacher_ID char(20),
	remark varchar(255));

drop table if exists student;
create table student (
	student_ID char(20) not null primary key,
	name char(20),
	sex tinyint(1),
	age int(8) unsigned,
	enroll_time datetime,
	address varchar(255),
	remark varchar(255));

drop table if exists teacher;
create table teacher (
	teacher_ID char(20) not null primary key,
	name char(20),
	sex tinyint(1),
	age int(8),
	mobi_phone char(12),
	home_phone varchar(12),
	address varchar(255),
	remark varchar(255));

drop table if exists school_class;
create table school_class (
	class_ID char(20) not null primary key,
	school_ID char(20),
	remark varchar(255));