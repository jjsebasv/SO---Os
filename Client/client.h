#ifndef _CLIENT_H_
#define _CLIENT_H_

void apiAddStudent (char * name, char * average);
void apiDbDeleteStudent (char * name);
void apiUpdateStudent (char * currentName, char * newName, double average);

#endif
