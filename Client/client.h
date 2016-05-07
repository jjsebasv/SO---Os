#ifndef _CLIENT_H_
#define _CLIENT_H_

void sendRequest(Request * request);
void apiAddStudent (char * name, double average);
void apiDeleteStudent (char * name);
void apiUpdateStudent (char * currentName, char * newName, double average);

#endif
