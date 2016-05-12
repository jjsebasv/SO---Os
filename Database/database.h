#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

sqlite3* openDatabase(void);
int checkTableExistance(sqlite3* db);
int createTable(sqlite3* db);
int writeDatabase(char name[25], char average[5]);

#endif
