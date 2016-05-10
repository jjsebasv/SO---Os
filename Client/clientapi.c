#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "../Api/request.h"
#include "client.h"
#include "../Api/commons.h"

// checked
void apiAddStudent (char * name, double average) {
	Student *student = malloc(sizeof(Student));
	Connection *connection = malloc(sizeof(Connection));

	student->name = name;
	student->average = average;
	//printf("Student->Name: %s\n", student->name);
	//printf("Student->average: %f\n", student->average);
	requestServer(connection, ADD_STUDENT, sizeof(student), student);
	getResponse(connection);
}

void apiDeleteStudent (char * name) {
	Request *request = malloc(sizeof(Request));
	//request = createRequest(DELETE_STUDENT, sizeof(*(name)), name);
	sendRequest(request);
}

void apiUpdateStudent (char * currentName, char * newName, double average) {
	apiDeleteStudent(currentName);
	apiAddStudent(newName, average);
}
