#ifndef _REQUEST_H_
#define _REQUEST_H_

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include "commons.h"

typedef enum {REQUEST_OK = 200, REQUEST_INVALID_TYPE, FAILED_ON_CREATE_REQUEST} requestState;

// Type - whether is a file / IP / PIPE / Named Pipe
// Direction - where to look for the data
// Action - whethet to read or write

typedef struct Request {
  int action;
  size_t dataSize;
  void* data;
  size_t directionSize;
  void* direction;
} Request;


//the client should use this function to start a request
//request is initialized and sent to the server
int requestServer(Connection * connection, int action, int type, size_t dataSize, void * data);

// Write a request in the request queue
requestState writeRequest(Request * request, Connection * connection);

// Get the first request in the request queue
int getRequest(Request * request);

// Process a request
int processRequest(Request * r);

// Gets the Connection from the server
int getConnection(Connection * connection);

Request * createRequest(int action, int type, size_t dataSize, void *data);

requestState readRequest(Request r);

requestState deleteRequest(Request r);

void createConnection(Connection * connection);

#endif
