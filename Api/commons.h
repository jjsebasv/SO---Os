
#ifndef _COMMONS_H_
#define _COMMONS_H_

typedef enum { ADD_STUDENT, ADD_STUDENT_AND_AVERAGE,
								UPDATE_STUDENT,
							 	DELETE_STUDENT } Action;

// creo que no necesitamos mas el typedef de connections
typedef enum connections { PIPE = 0, SOCKET, NAMED_PIPE, IP} connectionType;

typedef requestStates { REQUEST_OK = 200, REQUEST_INVALID_TYPE, FAILED_ON_CREATE_REQUEST } requestState;

#endif
