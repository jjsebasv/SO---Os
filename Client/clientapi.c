#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "client.h"
#include "../Api/commons.h"

// OPTION 1
void apiAddStudent (char * name, char * average) {
	Student *student = malloc(sizeof(Student));
	strcpy(student->name, name);
	strcpy(student->average, average);
	Connection *connection = malloc(sizeof(Connection));
	printf("SIZE DE LA ESTRUCTURA: %lu\n", sizeof((*student)));
	requestServer(connection, ADD_STUDENT, sizeof((*student)), student);
	//printf("FD EN APIADDSTUDENT%d\n", connection->fd);
	getResponse(connection);
}

// OPTION 3
void apiDbDeleteStudent (char * name) {
	Connection *connection = malloc(sizeof(Connection));
	requestServer(connection, DELETE_STUDENT, sizeof(name), name);
	getResponse(connection);
}

// OPTION 2
void apiUpdateStudent (char * currentName, char * newName, double average) {
	//apiDbDeleteStudent(currentName);
	//apiAddStudent(newName, average);
}
