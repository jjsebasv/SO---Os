#ifndef _COMMONS_H_
#define _COMMONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>

#define MAX_NAME_CHARACTERS 100
#define NOT_FOUND ((void*)0)


typedef enum { HELP,
              ADD_STUDENT,
              UPDATE_STUDENT,
              DELETE_STUDENT,
              READ_STUDENTS,
              DROP_TABLE,
              CREATE_TABLE,
            } Action;

typedef enum { ERROR_CREATE_SERVER_RESPONSE_RECIEVER = 400, ERROR_OPEN_REQUEST_QUEUE, SUCCESS, ERROR, NOT_FOUND_ERR, O_READONLY } connectionStates;
typedef enum {REQUEST_OK = 200, REQUEST_INVALID_TYPE, FAILED_ON_CREATE_REQUEST} requestState;

typedef struct Student {
  char name[MAX_NAME_CHARACTERS];
  char average[5];
} Student;

typedef struct Connection {
  int fd;
  int dataSize;
  Student * data;
} Connection;

typedef struct Request {  
  int action;
  Connection * connection;
} Request;

// Write a request in the request queue
requestState writeRequest(Request * request, int fd);

int processRequest(Request * request);
requestState readRequest(Request request);
requestState deleteRequest(Request request);

//OPENS REQUEST QUEUE
Connection * openConnection();

// Get the first request in the request queue
Request * getRequest(Connection * connection, int listened);

void monitorConnection(Connection * connection, fd_set* set);

int requestServer(Connection * connection, int action, int dataSize, void * data);
int getResponse(Connection * connection);
int listenConnection(Connection * connection);

#endif