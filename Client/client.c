#include <stdio.h>

#include "../Api/commons.h"
#include "client.h"

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
	apiAddStudent(name, averageArr);
}

static void deleteStudent () {
	char name[MAX_NAME_CHARACTERS];
	printf("%s", STUDENT_NAME_MESSAGE);
	scanf("%s" , name);
	apiDeleteStudent(name);
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

	apiUpdateStudent(currentName, newName, averageArr);
}

static void help () {
	printf("%s", ADD_STUDENT_HELP);
	printf("%s", UPDATE_STUDENT_HELP);
	printf("%s", DELETE_STUDENT_HELP);
	printf("%s", READ_STUDENTS_HELP);
	printf("%s", DROP_STUDENTS_TABLE_HELP);
	printf("%s", CREATE_STUDENTS_TABLE_HELP);
}

static void dropTable () {
	apiDropTable();
}

static void createTable () {
	apiCreateTable();
}

static void readStudents () {
	apiReadStudents();
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

		case CREATE_TABLE:
			createTable();
			break;

		case HELP:
			help();
			break;

		default:
			cleanBuffer();
			invalidCommand();
	}
}

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
