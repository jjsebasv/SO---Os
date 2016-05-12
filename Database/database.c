#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "database.h"

int printRow(void *NotUsed, int argc, char **argv, char **azColName) {
  NotUsed = 0;
  for (int i = 0; i < argc; i=i+2) {
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

int DbCreateTable(sqlite3* db) {
  char *err_msg = 0;
  int rc = sqlite3_exec(db, SQL_CREATE_TABLE, 0, 0, &err_msg);
  if (rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    fprintf(stderr, "SQL error: %d\n", rc);
    sqlite3_free(err_msg);
    return -1;
  }
  return rc;
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
    printf("%s\n", DATABASE_ERROR);
    return -1;
  }

	char *err_msg = 0;


	char sql[200] = "insert into Students values('";
	strcat(sql, name);
  strcat(sql, APOSTROPHE);
	strcat(sql, COMMA);
	strcat(sql, average);
	strcat(sql, LAST_P);

  printf("%s\n", sql);

	rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

	if (rc != SQLITE_OK ) {
    if ( rc == USER_EXISTS ) {
      printf("%s\n", USER_EXISTS_ERROR_MESSAGE);
    } 
    else {
      fprintf(stderr, "SQL error: %s\n", err_msg);
      fprintf(stderr, "SQL error: %d\n", rc);
    }
		sqlite3_free(err_msg);
		sqlite3_close(db);
	}
	sqlite3_close(db);
	return 0;
}

int DbReadStudents (){
  int rc;
  sqlite3* db = DbOpen();
  rc = DbCeckTableExistance(db);

  if ( db == NULL || rc == -1 ) {
    sqlite3_close(db);
    printf("%s\n", DATABASE_ERROR);
    return -1;
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
  return 0;
}

int DbDeleteStudent (char name[25]){

  int rc;
  sqlite3* db = DbOpen();
  rc = DbCeckTableExistance(db);

  if ( db == NULL || rc == -1 ) {
    sqlite3_close(db);
    printf("%s\n", DATABASE_ERROR);
    return -1;
  }

  char *err_msg = 0;

  char sql[200] = "DELETE FROM Students where Name='";
  strcat(sql, name);
  strcat(sql, APOSTROPHE);
  strcat(sql, SEMILCOLON);
  
  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

  if (rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    fprintf(stderr, "SQL error: %d\n", rc);
    sqlite3_free(err_msg);
    sqlite3_close(db);
  }

  sqlite3_close(db);
  return 0;
}

int DbUpdateStudent (char currentName[25], char newName[25], char average[5]) {
  int rc;
  sqlite3* db = DbOpen();
  rc = DbCeckTableExistance(db);

  if ( db == NULL || rc == -1 ) {
    sqlite3_close(db);
    printf("%s\n", DATABASE_ERROR);
    return -1;
  }

  char *err_msg = 0;

  // Maybe we should use this
  // printf("%s\n", SQL_UPDATE_STUDENT(mynameis, newName, average));

  char sql[200] = "UPDATE Students SET Name='";
  strcat(sql, newName);
  strcat(sql, APOSTROPHE);
  strcat(sql, COMMA);
  strcat(sql, "average='");
  strcat(sql, average);
  strcat(sql, APOSTROPHE);
  strcat(sql, " WHERE Name='");
  strcat(sql, currentName);
  strcat(sql, APOSTROPHE);
  strcat(sql, SEMILCOLON);
  
  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

  if (rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    fprintf(stderr, "SQL error: %d\n", rc);
    sqlite3_free(err_msg);
    sqlite3_close(db);
  }

  sqlite3_close(db);
  return 0;
}

int DbDropTable (char tableName[25]) {

  int rc;
  sqlite3* db = DbOpen();
  rc = DbCeckTableExistance(db);

  if ( db == NULL || rc == -1 ) {
    sqlite3_close(db);
    printf("%s\n", DATABASE_ERROR);
    return -1;
  }
  char *err_msg = 0;

  char sql[200] = "DROP TABLE '";
  strcat(sql, tableName);
  strcat(sql, APOSTROPHE);
  strcat(sql, SEMILCOLON);

  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

  if (rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    fprintf(stderr, "SQL error: %d\n", rc);
    sqlite3_free(err_msg);
    sqlite3_close(db);
  }

  sqlite3_close(db);
  return 0;
}




