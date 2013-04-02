#if !defined(DATABASE_H__)
#define DATABASE_H__

#include "RFID prototypeDlg.h"
#include "config.h"

extern char send_data[1024];
BOOL CheckDataBaseExist();
BOOL CloseDatabase();
BOOL InsertBusDepart(struct bus_depart &Bus_Depart);
BOOL InsertBusStu(struct bus_stu &Bus_Student);
BOOL DeleteBusDepart(char *bus_run_ID);
BOOL DeleteBusStu(char *bus_run_ID, char *student_ID);
BOOL TestDatabase();
BOOL QueryBusDepart(char *bus_run_ID, struct bus_depart *bus_depart);
BOOL QueryBusStu(char *bus_run_ID, char *student_ID, struct bus_stu *bus_stu);
BOOL InsertSchoolBus(StructPerson strtt, CString cp);
BOOL UpdateSchoolBusUp(StructPerson sp);
BOOL UpdateSchoolBusDown(StructPerson sp, CString time);

#endif