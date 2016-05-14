#ifndef _CLIENT_H_
#define _CLIENT_H_

void apiAddStudent (char * name, char * average);
void apiDbDeleteStudent (char * name);
void apiUpdateStudent (char * currentName, char * newName, double average);

#define ADD_STUDENT_HELP "(1) Add a new student\n"
#define UPDATE_STUDENT_HELP "(2) Update a student\n"
#define DELETE_STUDENT_HELP "(3) Delete a student\n"
#define READ_STUDENTS_HELP "(4) Read students\n"
#define DROP_STUDENTS_TABLE_HELP "(5) Drop students table\n"
#define STUDENT_NAME_MESSAGE "Student name:\t"
#define STUDENT_AVERAGE_MESSAGE "Student average:\t"
#define STUDENT_CURRENT_NAME_MESSAGE "Current student name:\t"
#define INVALID_COMMAND	"Invalid command:\n"
#define WELCOME "Welcome to giladita system\n"
#define ENTER_COMMAND "Please enter your command (enter 0 for help): "

#endif
