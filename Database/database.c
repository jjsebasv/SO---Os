#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "../Api/commons.h"
#include "../Api/namedPipe/namedPipe.h"
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
}

int main (int argc, char const *argv[]){

	fd_set set;  	//fds to monitor
	Request * r;
	struct timeval tv;
	Connection* c;

	// //initialize request queue TODO REDO
	if( (c = openConnection(SQL_QUEUE)) == NOT_FOUND) {
		return ERROR_OPEN_REQUEST_QUEUE;
	}
	//sets up the fds to monitor
	monitorConnection(c, &set);

	//waits 2 seconds
	tv.tv_sec = 2;
  	tv.tv_usec = 0;

	while(1) {
		int fdCount = select(c -> fd + 1, &set, NULL, NULL, &tv);
		tv.tv_sec = 2;
		monitorConnection(c, &set);
		if( fdCount > 0){
			r = getRequest(c);
			if( r != NOT_FOUND ){
				printf("Request procesada\n");
				processRequestDatabase (r);
			}
		} else if ( fdCount < 0 ){
			printf("Error\n");
		} else {
			printf("No requests to process\n");
		}
	}

	return 0;
}
