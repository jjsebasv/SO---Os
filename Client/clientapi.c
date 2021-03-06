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
	requestServer(connection, ADD_STUDENT, sizeof((*student)), student);
	getResponse(connection);
}

// OPTION 3
void apiDeleteStudent (char * name) {
	Connection *connection = malloc(sizeof(Connection));
	requestServer(connection, DELETE_STUDENT, sizeof(name), name);
	getResponse(connection);
}

// OPTION 2
void apiUpdateStudent (char * currentName, char * newName, char * average) {
	apiDeleteStudent(currentName);
	apiAddStudent(newName, average);
}

void apiDropTable() {
	Connection *connection = malloc(sizeof(Connection));
	requestServer(connection, DROP_TABLE , 0, NULL);
	getResponse(connection);
}

void apiCreateTable() {
	Connection *connection = malloc(sizeof(Connection));
	requestServer(connection, CREATE_TABLE , 0, NULL);
	getResponse(connection);
	getResponse(connection);
}

void apiReadStudents() {
	Connection *connection = malloc(sizeof(Connection));
	requestServer(connection, READ_STUDENTS, 0, NULL);
	getResponse(connection);
	printStudents();
}

void printStudents() {
	printf("Name\tAverage\n");
	int c;
	FILE *file;
	file = fopen("dababaseExport.txt", "r");
	if (file) {
    while ((c = getc(file)) != EOF)
      putchar(c);
    fclose(file);
	}
}
