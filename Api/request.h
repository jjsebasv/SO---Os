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
  Connection * connection;
} Request;


// Write a request in the request queue
requestState writeRequest(Request * request, int fd);

// Get the first request in the request queue
Request * getRequest(Connection * connection);

// Process a request
int processRequest(Request * request);

requestState readRequest(Request request);

requestState deleteRequest(Request request);

#endif
