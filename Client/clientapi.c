#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "../Api/request.h"
#include "client.h"
#include "../Api/commons.h"

// toma bien los datos
void apiAddStudent (char * name, double average) {
	Student *student = malloc(sizeof(Student));
	Request *request = malloc(sizeof(Request));

	student->name = name;
	student->average = average;
	//printf("Student->Name: %s\n", student->name);
	//printf("Student->average: %f\n", student->average);

	request = malloc(sizeof(request));
	request = createRequest(ADD_STUDENT, sizeof(student), student);
	sendRequest(request);
}

void apiDeleteStudent (char * name) {
	Request *request;
	request = createRequest(DELETE_STUDENT, sizeof(*(name)), name);
	sendRequest(request);
}

void apiUpdateStudent (char * currentName, char * newName, double average) {
	apiDeleteStudent(currentName);
	apiAddStudent(newName, average);
}
