#include <stdio.h>
#include "request.h"

#define NOT_FOUND_ERR   NULL

int requestServer(Connection * connection, int action, int type, size_t dataSize, void * data) {
  Request * request = createRequest(action, type, dataSize, data);

  // REDO WITHOUT NAME PIPES
  int fd[2];

  if(request == NULL){
    printf("Failed to create request\n");   //no esta bueno poner printf en backend pero nos va a servir para debugear.
    return FAILED_ON_CREATE_REQUEST;                           //despues los borro
  }

  request->direction = fd[1];
  request->directionSize = sizeof(int);
  writeRequest(request, connection);
  createConnection(connection);
  if(connection == NULL){
    return NULL;
  }
  connection->np = fd[0];
  return SUCCESS; 
}

int getConnection (Connection * connection) {
  return 0;
}
