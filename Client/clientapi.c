#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "include/commons.h"

static void sendRequest(Request * request) {
	writeRequest(request);
}

/**
 *	Add a student to the data base
 *
 *	@param name: student's name
 */
static void addStudent (char * name) {
	Student student;
	student.name = name;

	Request request;
	request.action = ADD_STUDENT;
	request.data = student;
	request.dataSize = sizeof(student);
	// direction & direction size?
	sendRequest(&request);
}

/**
 *	Add a student to the data base
 *
 *	@param name: student's name
 *	@param average: student's average
 */
static void addStudent (char * name, double average) {
	Student student;
	student.name = name;
	student.average = average;
	Request request;
	request.action = ADD_STUDENT_AND_AVERAGE;
	request.data = student;
	request.dataSize = sizeof(student);
	// direction & direction size?
	sendRequest(&request);
}


/**
 *	Delete a student from the data base
 *
 *	@param name: student's name
 */
static void deleteStudent (char * name) {
	Request request;
	request.action = DELETE_STUDENT;
	request.data = name;
	request.dataSize = (*(name));
	sendRequest(&request);
}


/**
 *	Delete a student from the data base
 *
 *	@param currentName:
 *	@param newName:
 *	@param :
 */
static void updateStudent (char * currentName, char * newName, double average) {
	deleteStudent(currentName);
	addStudent(newName, average);
}