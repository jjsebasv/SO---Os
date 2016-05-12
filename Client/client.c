#include <stdio.h>

#include "../Api/commons.h"
#include "../Api/request.h"
#include "../Database/database.h"
#include "client.h"

#define ADD_STUDENT_HELP "(1) Add a new student\n"
#define UPDATE_STUDENT_HELP "(2) Update a student\n"
#define DELETE_STUDENT_HELP "(3) Delete a student\n"
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
	printf("mira aca --> %s\n", averageArr);
	writeDatabase(name, averageArr);

	apiAddStudent(name, average);
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

	apiUpdateStudent(currentName, newName, average);
}

static void help () {
	printf("%s", ADD_STUDENT_HELP);
	printf("%s", UPDATE_STUDENT_HELP);
	printf("%s", DELETE_STUDENT_HELP);
}

static void invalidCommand () {
	printf("%s\n", INVALID_COMMAND);
}

// TO DO WITHOUT SWTICH

// int processRequest(Request * r) {
//   int to_execute = r -> action;
//   int (*functionChooser[3]) (Request * r);

//   functionChooser[0] = readRequest;
//   functionChooser[1] = writeRequest;
//   functionChooser[2] = deleteRequest;

//   if (to_execute != UPDATE_STUDENT && to_execute != ADD_STUDENT && to_execute != DELETE_STUDENT)
//     return ERROR;

//   return (*functionChooser[to_execute])(r);
// }


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

		case HELP:
			help();
			break;

		default:
			cleanBuffer();
			invalidCommand();
	}
}

// Sqlite
/*
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
  	printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}
*/

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
