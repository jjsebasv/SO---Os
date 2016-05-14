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
				READ_STUDENTS,
				DROP_TABLE,
				} Action;

typedef enum { ERROR_CREATE_SERVER_RESPONSE_RECIEVER = 400, ERROR_OPEN_REQUEST_QUEUE, SUCCESS, ERROR, NOT_FOUND_ERR, O_READONLY } connectionStates;
typedef enum {REQUEST_OK = 200, REQUEST_INVALID_TYPE, FAILED_ON_CREATE_REQUEST} requestState;

typedef struct Student {
  char * name;
  char * average;
} Student;

typedef struct Connection {
  int fd;
  int dataSize;
  void * data;
} Connection;


typedef struct Request {	
  int action;
  Connection * connection;
} Request;

// Write a request in the request queue
requestState writeRequest(Request * request, int fd);

// Get the first request in the request queue
Request * getRequest(Connection * connection);

int processRequest(Request * request);
requestState readRequest(Request request);
requestState deleteRequest(Request request);

//OPENS REQUEST QUEUE
Connection* openConnection (void);

void monitorConnection(Connection * connection, fd_set* set);

int requestServer(Connection * connection, int action, int dataSize, void * data);
int getResponse(Connection * connection);

#endif
