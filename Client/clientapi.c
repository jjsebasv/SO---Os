#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "../Api/request.h"
#include "client.h"
#include "../Api/commons.h"

// OPTION 1
void apiAddStudent (char * name, double average) {
	Student *student = malloc(sizeof(Student));

	student->name = name;
	student->average = average;
	//printf("Student->Name: %s\n", student->name);
	//printf("Student->average: %f\n", student->average);
	//requestServer(connection, ADD_STUDENT, sizeof(student), student);
	//getResponse(connection);

}

// OPTION 3
void apiDbDeleteStudent (char * name) {
	Connection *connection = malloc(sizeof(Connection));
	requestServer(connection, DELETE_STUDENT, sizeof(name), name);
	getResponse(connection);
}

// OPTION 2
void apiUpdateStudent (char * currentName, char * newName, double average) {
	apiDbDeleteStudent(currentName);
	apiAddStudent(newName, average);
}
