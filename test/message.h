#ifndef _message_h_
#define _message_h_
#include "common.h"
#include "database.h"

bool parseBusDepart(char *msg, struct update *update);
bool parseBusRun(char *msg, struct update *update);
bool parseBusStudent(char *msg, struct update *update);
int BusRunInfoQueryRight(CArray<rowItem, rowItem> &rest);
int BusDepartInfoQueryRight(CArray<rowItem, rowItem> &rest);
int BusStudentInfoQueryRight(CArray<rowItem, rowItem> &rest);

#endif