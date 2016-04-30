#include <stdio.h>

#define ADD_STUDENT_HELP "(1) Add a new student\n"
#define UPDATE_STUDENT_HELP "(2) Update a student\n"
#define DELETE_STUDENT_HELP "(3) Delete a student\n"
#define STUDENT_NAME_MESSAGE "Student name:\t"
#define STUDENT_NAME_MESSAGE "Student average:\t"
#define STUDENT_CURRENT_NAME_MESSAGE "Current student name:\t"
#define INVALID_COMMAND	"Invalid command:\n"
#define MAX_NAME_CHARACTERS 100
#define WELCOME "Welcome to giladita system\n"
#define ENTER_COMMAND "Please enter your command (enter 0 for help):\n"


static void getCommand (int command) {
	swich (command) {

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
			invalidCommand();
	}
}

static void addStudent () {
	char name[MAX_NAME_CHARACTERS];
	printf("%s\n", STUDENT_NAME_MESSAGE);
	scanf("%s" , name);

	printf("%s\n", STUDENT_AVERAGE_MESSAGE);
	double average = getdouble();

	apiAddStudent(name, average);
}

static void deleteStudent () {
	char name[MAX_NAME_CHARACTERS];
	printf("%s\n", STUDENT_NAME_MESSAGE);
	scanf("%s" , name);
	apiDeleteStudent(name);
}

static void updateStudent () {
	char currentName[MAX_NAME_CHARACTERS];
	char newName[MAX_NAME_CHARACTERS];

	printf("%s\n", STUDENT_CURRENT_NAME_MESSAGE);
	scanf("%s" , currentName);

	printf("%s\n", STUDENT_NAME_MESSAGE);
	scanf("%s" , newName);

	printf("%s\n", STUDENT_AVERAGE_MESSAGE);
	double average = getdouble();

	apiUpdateStudent(currentName, newName, average);
}

static void help () {
	printf("%s\n", ADD_STUDENT_HELP);
	printf("%s\n", UPDATE_STUDENT_HELP);
	printf("%s\n", DELETE_STUDENT_HELP);
}

static void invalidCommand () {
	printf("%s\n", INVALID_COMMAND);
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