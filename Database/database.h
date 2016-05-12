#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

#define DATABASE_NAME "database.db"
#define NO_TABLE 404
#define USER_EXISTS 19
#define NO_SESSION NULL
#define COMMA "','"
#define LAST_P "');"
#define SEMILCOLON ";"
#define APOSTROPHE "'"
#define SELECT_ALL "select * from Students;"
#define USER_EXISTS_ERROR_MESSAGE "That user is already registered"
#define MISSING_TABLE 1
#define SQL_CREATE_TABLE "CREATE TABLE Students(Name varchar[25] PRIMARY KEY, Average float);"

sqlite3* DbOpen(void);
int DbCeckTableExistance(sqlite3* db);
int DbCreateTable(sqlite3* db);
int DbAddStudent(char name[25], char average[5]);
int DbDeleteStudent (char name[25]);
int DbReadStudents ();
int printRow (void *NotUsed, int argc, char **argv, char **azColName);
int DbDropTable (char tableName[25]);
int DbDropDatabase (char databaseName[25]);
int DbUpdateStudent (char currentName[25], char newName[25], char average[5]);

#endif
