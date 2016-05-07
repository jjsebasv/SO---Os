#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "../Api/request.h"
#include "client.h"
#include "../Api/commons.h"

void sendRequest(Request * request) {
	// escribir la request en la queue
}

void apiAddStudent (char * name, double average) {
	Student *student = malloc(sizeof(Student));
	student->name = name;
	student->average = average;
	Request request;
	request.action = ADD_STUDENT;
	request.data = student;
	request.dataSize = sizeof(student);
	// direction & direction size?
	sendRequest(&request);
}

void apiDeleteStudent (char * name) {
	Request request;
	request.action = DELETE_STUDENT;
	request.data = name;
	request.dataSize = (*(name));
	sendRequest(&request);
}

void apiUpdateStudent (char * currentName, char * newName, double average) {
	apiDeleteStudent(currentName);
	apiAddStudent(newName, average);
}
