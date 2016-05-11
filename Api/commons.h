#ifndef _COMMONS_H_
#define _COMMONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>

#define NOT_FOUND ((void*)0)

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

int requestServer(Connection * connection, int action, size_t dataSize, void * data);
int getResponse(Connection * connection);

#endif
