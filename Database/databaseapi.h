#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

#define DATABASE_NAME "database.db"
#define NO_TABLE 404
#define USER_EXISTS 19
#define NO_SESSION NULL
#define COMMA ","
#define LAST_P ");"
#define SEMILCOLON ";"
#define APOSTROPHE "'"
#define SQL_SELECT_ALL "select * from Students;"
#define USER_EXISTS_ERROR_MESSAGE "That user is already registered"
#define MISSING_TABLE 1
#define SQL_CREATE_TABLE "CREATE TABLE Students(Name varchar[25] PRIMARY KEY, Average float);"
#define SQL_UPDATE_STUDENT ("UPDATE Students SET Name='%s', Average='%s' WHERE Name='%s'")
#define SQL_ADD_STUDENT ("insert into Students values('%s','%s');")
#define SQL_DELETE_STUDENT ("DELETE FROM Students where Name='%s'")
#define SQL_DROP_TABLE ("DROP TABLE 'Students'")

typedef enum { ADD_STUDENT_SUCCESS,
							ADD_STUDENT_USER_EXIST,
							UPDATE_STUDENT_SUCCESS,
							DELETE_STUDENT_SUCCESS,
							READ_STUDENTS_SUCCESS,
							DATABASE_ERROR,
	} sqlState;


sqlite3* DbOpen(void);
int DbCeckTableExistance(sqlite3* db);
int DbCreateTable();
int DbAddStudent(char name[25], char average[5]);
int DbDeleteStudent (char name[25]);
int DbReadStudents ();
int printRow (void *NotUsed, int argc, char **argv, char **azColName);
int DbDropTable ();
int DbDropDatabase (char databaseName[25]);
int DbUpdateStudent (char currentName[25], char newName[25], char average[5]);

#endif
