#ifndef _message_h_
#define _message_h_
#include "common.h"
#include "database.h"

bool parseBusDepart(char *msg, struct update *update);
bool parseBusRun(char *msg, struct update *update);
bool parseBusStudent(char *msg, struct update *update);

#endif