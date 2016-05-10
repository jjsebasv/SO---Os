#ifndef _COMMONS_H_
#define _COMMONS_H_

typedef enum { HELP,
				ADD_STUDENT,
				UPDATE_STUDENT,
				DELETE_STUDENT,
				} Action;

typedef enum { ERROR_CREATE_SERVER_RESPONSE_RECIEVER = 400, ERROR_OPEN_REQUEST_QUEUE, SUCCESS, ERROR, NOT_FOUND_ERR, O_READONLY } connectionStates;

typedef struct Student {
  char * name;
  double average;
} Student;


typedef struct NPConnection {
  int fd;
  int dataSize;
  char* data;
} NPConnection;

//TODO
typedef struct SConnection {
} STConnection;

typedef union Connection {
	NPConnection * np;
	STConnection * st;
} Connection;

//OPENS REQUEST QUEUE
Connection* openConnection (void);

void monitorConnection(Connection * connection, fd_set* set);

#endif
