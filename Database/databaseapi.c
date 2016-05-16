#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "databaseapi.h"

int printRow(void *NotUsed, int argc, char **argv, char **azColName) {
  NotUsed = 0;
  int i;
  for (i = 0; i < argc; i=i+2) {
      printf("%s\t%s", argv[i] ? argv[i] : "NULL", argv[i+1] ? argv[i+1] : "NULL");
  }
  printf("\n");
  return 0;
}

sqlite3* DbOpen(void) {
  sqlite3* db;
  int rc = sqlite3_open(DATABASE_NAME, &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return NO_SESSION;
  }
  return db;
}

int DbCreateTable() {
  sqlite3* db;
  int rc = sqlite3_open(DATABASE_NAME, &db);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return DATABASE_ERROR;
  }

  char *err_msg = 0;
  int req = sqlite3_exec(db, SQL_CREATE_TABLE, 0, 0, &err_msg);
  if (req != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    fprintf(stderr, "SQL error: %d\n", req);
    sqlite3_free(err_msg);
    return DATABASE_ERROR;
  }
  return CREATE_TABLE_SUCCESS;
}

int DbCeckTableExistance(sqlite3* db) {
  char *err_msg = 0;
  int rc = sqlite3_exec(db, SQL_SELECT_ALL, 0, 0, &err_msg);

  if (rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    fprintf(stderr, "SQL error: %d\n", rc);

    if ( rc == -1 ) {
      sqlite3_close(db);
    }
  }
  return rc;
}

int DbAddStudent(char name[25], char average[5]) {

  int rc;
  sqlite3* db = DbOpen();
  rc = DbCeckTableExistance(db);

  if ( db == NULL || rc == -1 ) {
    sqlite3_close(db);
    return DATABASE_ERROR;
  }

	char *err_msg = 0;

  char sql[200];
  sprintf (sql, SQL_ADD_STUDENT, name, average);

	rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

	if (rc != SQLITE_OK ) {
    if ( rc == USER_EXISTS ) {
      printf("%s\n", USER_EXISTS_ERROR_MESSAGE);
      return ADD_STUDENT_USER_EXIST;
    } else {
      fprintf(stderr, "SQL error: %s\n", err_msg);
      fprintf(stderr, "SQL error: %d\n", rc);
      return DATABASE_ERROR;
    }
		sqlite3_free(err_msg);
		sqlite3_close(db);
	}
	sqlite3_close(db);
	return ADD_STUDENT_SUCCESS;
}

int DbReadStudents (){
  int rc;
  sqlite3* db = DbOpen();
  rc = DbCeckTableExistance(db);

  if ( db == NULL || rc == -1 ) {
    sqlite3_close(db);
    return DATABASE_ERROR;
  }

  char *err_msg = 0;

  printf("Name\tAverage\n");
  rc = sqlite3_exec(db, SQL_SELECT_ALL  , printRow, 0, &err_msg);

  if (rc != SQLITE_OK ) {
    fprintf(stderr, "Failed to select data\n");
    fprintf(stderr, "SQL error: %s\n", err_msg);

    sqlite3_free(err_msg);
    sqlite3_close(db);
    
    return 1;
  } 
  
  sqlite3_close(db);
  return READ_STUDENTS_SUCCESS;
}

int DbDeleteStudent (char name[25]){

  int rc;
  sqlite3* db = DbOpen();
  rc = DbCeckTableExistance(db);

  if ( db == NULL || rc == -1 ) {
    sqlite3_close(db);
    return DATABASE_ERROR;
  }

  char *err_msg = 0;


  char sql[200];
  sprintf (sql, SQL_DELETE_STUDENT, name);
  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

  if (rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    fprintf(stderr, "SQL error: %d\n", rc);
    sqlite3_free(err_msg);
    sqlite3_close(db);
  }

  sqlite3_close(db);
  return DELETE_STUDENT_SUCCESS;
}

int DbUpdateStudent (char currentName[25], char newName[25], char average[5]) {
  int rc;
  sqlite3* db = DbOpen();
  rc = DbCeckTableExistance(db);

  if ( db == NULL || rc == -1 ) {
    sqlite3_close(db);
    return DATABASE_ERROR;
  }

  char *err_msg = 0;

  char sql[200];
  sprintf (sql, SQL_UPDATE_STUDENT, newName, average, currentName);
  
  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

  if (rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    fprintf(stderr, "SQL error: %d\n", rc);
    sqlite3_free(err_msg);
    sqlite3_close(db);
  }

  sqlite3_close(db);
  return UPDATE_STUDENT_SUCCESS;
}

int DbDropTable () {

  int rc;
  sqlite3* db = DbOpen();
  rc = DbCeckTableExistance(db);

  if ( db == NULL || rc == -1 ) {
    sqlite3_close(db);
    return DATABASE_ERROR;
  }
  char *err_msg = 0;

  rc = sqlite3_exec(db, SQL_DROP_TABLE, 0, 0, &err_msg);

  if (rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    fprintf(stderr, "SQL error: %d\n", rc);
    sqlite3_free(err_msg);
    sqlite3_close(db);
  }

  sqlite3_close(db);
  return DROP_TABLE_SUCCESS;
}




