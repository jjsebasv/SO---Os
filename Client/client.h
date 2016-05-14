#ifndef _CLIENT_H_
#define _CLIENT_H_
#include "../Api/commons.h"

void sendRequest(Request * request);
void apiAddStudent (char * name, double average);
void apiDbDeleteStudent (char * name);
void apiUpdateStudent (char * currentName, char * newName, double average);

#endif
