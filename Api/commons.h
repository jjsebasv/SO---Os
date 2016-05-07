#ifndef _COMMONS_H_
#define _COMMONS_H_

typedef enum { HELP,
				ADD_STUDENT,
				UPDATE_STUDENT,
				DELETE_STUDENT,
				} Action;

typedef enum { REQUEST_OK = 200, REQUEST_INVALID_TYPE, FAILED_ON_CREATE_REQUEST } requestState;

typedef struct Student {
  char * name;
  double average;
} Student;

#endif
