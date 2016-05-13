#include <stdio.h>

#include "../Api/commons.h"
#include "../Api/request.h"
#include "../Database/database.h"
#include "client.h"

#define ADD_STUDENT_HELP "(1) Add a new student\n"
#define UPDATE_STUDENT_HELP "(2) Update a student\n"
#define DELETE_STUDENT_HELP "(3) Delete a student\n"
#define READ_STUDENTS_HELP "(4) Read students\n"
#define DROP_STUDENTS_TABLE_HELP "(5) Drop students table\n"
#define STUDENT_NAME_MESSAGE "Student name:\t"
#define STUDENT_AVERAGE_MESSAGE "Student average:\t"
#define STUDENT_CURRENT_NAME_MESSAGE "Current student name:\t"
#define INVALID_COMMAND	"Invalid command:\n"
#define MAX_NAME_CHARACTERS 100
#define WELCOME "Welcome to giladita system\n"
#define ENTER_COMMAND "Please enter your command (enter 0 for help): "

static void cleanBuffer () {
	char c;
	while ( (c = getchar()) != '\n' && c != EOF );
}

static void addStudent () {
	char name[MAX_NAME_CHARACTERS];
	printf("%s", STUDENT_NAME_MESSAGE);
	scanf("%s" , name);
	printf("%s", STUDENT_AVERAGE_MESSAGE);
	double average;
	scanf("%lf",&average);
	char averageArr[5];
	sprintf(averageArr, "%g", average);

	// Replace DbAddStudent with apiAddStudent
	apiAddStudent(name, average);
	// DbAddStudent(name, averageArr);
}

static void deleteStudent () {
	char name[MAX_NAME_CHARACTERS];
	printf("%s", STUDENT_NAME_MESSAGE);
	scanf("%s" , name);

	// Replace DbDeleteStudent with apiDeleteStudent
	//apiDeleteStudent(name);
	DbDeleteStudent(name);
}

static void updateStudent () {
	char currentName[MAX_NAME_CHARACTERS];
	char newName[MAX_NAME_CHARACTERS];

	printf("%s", STUDENT_CURRENT_NAME_MESSAGE);
	scanf("%s" , currentName);

	printf("%s", STUDENT_NAME_MESSAGE);
	scanf("%s" , newName);

	printf("%s", STUDENT_AVERAGE_MESSAGE);
	double average;
	scanf("%lf",&average);
	char averageArr[5];
	sprintf(averageArr, "%g", average);

	// Replace DbUpdateStudent with apiUpdateStudent
	// apiUpdateStudent(currentName, newName, average);
	DbUpdateStudent (currentName, newName, averageArr);
}

static void help () {
	printf("%s", ADD_STUDENT_HELP);
	printf("%s", UPDATE_STUDENT_HELP);
	printf("%s", DELETE_STUDENT_HELP);
	printf("%s", READ_STUDENTS_HELP);
	printf("%s", DROP_STUDENTS_TABLE_HELP);
}

static void dropTable () {
	char * tableName = "Students";
	// Replace DbDropTable with apiDropTable
	// apiDropTable();
	DbDropTable(tableName);
}

static void readStudents () {
	// Replace DbReadStudents with apiReadStudents
	// apiReadStudents();
	DbReadStudents();
}

static void invalidCommand () {
	printf("%s\n", INVALID_COMMAND);
}

static void getCommand (int command) {

	switch (command) {

		case ADD_STUDENT:
			addStudent();
			break;

		case UPDATE_STUDENT:
			updateStudent();
			break;

		case DELETE_STUDENT:
			deleteStudent();
			break;

		case READ_STUDENTS:
			readStudents();
			break;

		case DROP_TABLE:
			dropTable();
			break;

		case HELP:
			help();
			break;

		default:
			cleanBuffer();
			invalidCommand();
	}
}

// MAIN

int main() {
	int command;

	printf("%s\n", WELCOME);

	while (1) {
		command = 0;

		printf(ENTER_COMMAND);
		scanf("%d", &command);
		getCommand(command);
	}

	return 0;
}
