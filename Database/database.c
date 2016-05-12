#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

#define NO_TABLE 404
#define USER_EXISTS 19
#define NO_SESSION NULL


sqlite3* openDatabase(void) {
  sqlite3* db;
  int rc = sqlite3_open("database.db", &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return NO_SESSION;
  }
  return db;
}

int createTable(sqlite3* db) {
  char *err_msg = 0;
  char *sql = "CREATE TABLE Students(Name varchar[25] PRIMARY KEY, Average float);";
  int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    fprintf(stderr, "SQL error: %d\n", rc);
    sqlite3_free(err_msg);
    return -1;
  }
  return rc;
}

int checkTableExistance(sqlite3* db) {
  char *err_msg = 0;
  char* sql = "select * from Students;";
  int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    fprintf(stderr, "SQL error: %d\n", rc);

    if ( rc == 1 ) { // No table error
      rc = createTable(db);
      if ( rc == -1 )
        sqlite3_close(db);
    }
  }
  return rc;
}

int writeDatabase(char name[25], char average[5]) {

  int rc;
  sqlite3* db = openDatabase();
  rc = checkTableExistance(db);

  if ( db == NULL || rc == -1 ) {
    sqlite3_close(db);
    printf("DATABASE ERROR\n");
    return -1;
  }

	char *err_msg = 0;

	char sql[200] = "insert into Students values('";
	char* comma = "','";
	char* last_p = "');";
	strcat(sql, name);
	strcat(sql, comma);
	strcat(sql, average);
	strcat(sql, last_p);

	rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

	if (rc != SQLITE_OK ) {
    if ( rc == USER_EXISTS ) {
      printf("That user is already registered\n");
    } else {
      fprintf(stderr, "SQL error: %s\n", err_msg);
      fprintf(stderr, "SQL error: %d\n", rc);
    }
		sqlite3_free(err_msg);
		sqlite3_close(db);
	}
	sqlite3_close(db);
	return 0;
}

// TODO
// readDatabase
// deleteFromDatabase
// updateDatabase
