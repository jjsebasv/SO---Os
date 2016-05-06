
#ifndef _COMMONS_H_
#define _COMMONS_H_

typedef enum { ADD_STUDENT, ADD_STUDENT_AND_AVERAGE,
								UPDATE_STUDENT,
							 	DELETE_STUDENT } Action;

typedef requestStates { REQUEST_OK = 200, REQUEST_INVALID_TYPE, FAILED_ON_CREATE_REQUEST } requestState;

typedef struct Student {
  char * name;
  double average;
} Student;

#endif
