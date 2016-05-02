#include "ipc.h"
#include "request.h"

#define NOT_FOUND_ERR   NULL
enum connectionsErrors {ERROR_CREATE_SERVER_RESPONSE_RECIEVER = 400, ERROR_OPEN_REQUEST_QUEUE} connectionError;

int requestServer(Response * response, int action, int type, size_t dataSize, void * data) {
  Request * request = createRequest(action, type, dataSize, data);

  if(request == NULL){
    printf("Failed to create request\n");   //no esta bueno poner printf en backend pero nos va a servir para debugear.
    return FAILED_ON_CREATE_REQUEST;                           //despues los borro
  }

  if(pipe(fd) != 0){
    printf("Cannot create server reponse receiver\n");
    return ERROR_CREATE_SERVER_RESPONSE_RECIEVER;
  }
  request->direction = fd[1];
  request->directionSize = sizeof(int);
  writeRequest(request);
  createResponse(response,fd[0]);
  if(response == NULL){
    return NULL;
  }
  return SUCCESS; 
}

//TODO
int getReponse(Response * response){
  return 0;
}

//TODO We should redo this function. it doesn't work for structures of different sizes
int getRequest(Request * request) {
  int aux_err;
  int fd = 0;
  //TODO open pipe with its name and start reading from it
  aux_err = read( fd, request, sizeof( Request ) );    
  if ( aux_err )
    return ERROR;
  return NOT_FOUND_ERR; // return NULL
}

//TODO Change switch to array of void * functions
int processRequest(Request * r) {
  switch (r -> action) {
    case READ:
      return readRequest(r);
      break;
    case CREATE:
      return writeRequest(r);
      break;
    case DELETE:
      return deleteRequest(r);
      break;
  }
  return ERROR;
}
