#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "client.h"
#include <unistd.h>
#include "../Api/commons.h"

void apiAddStudent (char * name, char * average) {
	Student *student = malloc(sizeof(Student));
	strcpy(student->name, name);
	strcpy(student->average, average);
	Connection *connection = malloc(sizeof(Connection));
	int fdAnswer = requestServer(connection, ADD_STUDENT, sizeof((*student)), student);
	Connection * connectionAnswer = malloc(sizeof(Connection));
	connectionAnswer-> fd = fdAnswer;
	getResponse(connectionAnswer);
}

void apiDeleteStudent (char * name) {
	Connection *connection = malloc(sizeof(Connection));
	requestServer(connection, DELETE_STUDENT, sizeof(name), name);
	// getResponse(connection);
}

void apiUpdateStudent (char * currentName, char * newName, char * average) {
	apiDeleteStudent(currentName);
	apiAddStudent(newName, average);
	// getResponse(connection);
}

void apiDropTable() {
	Connection *connection = malloc(sizeof(Connection));
	requestServer(connection, DROP_TABLE , 0, NULL);
	// getResponse(connection);
}

void apiCreateTable() {
	Connection *connection = malloc(sizeof(Connection));
	requestServer(connection, CREATE_TABLE , 0, NULL);
	// getResponse(connection);
}

void apiReadStudents() {
	Connection *connection = malloc(sizeof(Connection));
	requestServer(connection, READ_STUDENTS, 0, NULL);
	// getResponse(connection);
}