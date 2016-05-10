#include <stdio.h>
#include <stdlib.h>
#include "request.h"

#define NOT_FOUND ((void*)0)

// toma bien los datos
Request* createRequest(int action, size_t dataSize, void *data) {
  Request *request = malloc(sizeof(*request));
  request->action = action;
  request->dataSize = dataSize;
  request->data = data;
  return request;
}

void sendRequest(Request * request){
  printf("Send request:\n");
  // to do
  // writeRequestQueue namedPipe.c
  // writeRequestQueue socket.c
  // writeRequestQueue(request);
}

int requestServer(Connection * connection, int action, int type, size_t dataSize, void * data) {
  Request *request = createRequest(action, dataSize, data);

  // REDO WITHOUT NAME PIPES
  int fd[2];

  if(request == NOT_FOUND){
    printf("Failed to create request\n");   //no esta bueno poner printf en backend pero nos va a servir para debugear.
    return FAILED_ON_CREATE_REQUEST;                           //despues los borro
  }

  //request->direction = fd[1];
  //request->directionSize = sizeof(int);
  writeRequest(request, connection);
  createConnection(connection);
  if(connection == NOT_FOUND){
    return -1;
  }
  connection->np->fd = fd[0];
  return SUCCESS; 
}

int getConnection (Connection * connection) {
  return 0;
}

// TO DO
void createConnection(Connection * connection) {
  return;
}

