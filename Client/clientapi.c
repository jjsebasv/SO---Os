#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "include/commons.h"

static void sendRequest(Request * request) {
	// escribir la request en la queue
	writeRequest(request);
}

/**
 *	Add a student to the data base
 */
static void apiAddStudent (char * name, double average) {
	Student student;
	student.name = name;
	student.average = average;
	Request request;
	request.action = ADD_STUDENT;
	request.data = student;
	request.dataSize = sizeof(student);
	// direction & direction size?
	sendRequest(&request);
}

/**
 *	Delete a student from the data base
 */
static void apiDeleteStudent (char * name) {
	Request request;
	request.action = DELETE_STUDENT;
	request.data = name;
	request.dataSize = (*(name));
	sendRequest(&request);
}

/**
 *	Update a student from the data base
 */
static void apiUpdateStudent (char * currentName, char * newName, double average) {
	deleteStudent(currentName);
	addStudent(newName, average);
}
