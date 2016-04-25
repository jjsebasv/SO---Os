#include "ipc.h"
#include "request.h"

// donde van todos los enums y tydefs?
#define NOT_FOUND_ERR   NULL
enum connectionsErrors {ERROR_CREATE_SERVER_RESPONSE_RECIEVER = 400, ERROR_OPEN_REQUEST_QUEUE} connectionError;

int myRequest(int action, int type, size_t dataSize, void* data, int mode) {
  int fd[2];
  struct Request r = createRequest(action, type, dataSize, data);

  if(r == NULL){
    printf("Failed to create request\n");   //no esta bueno poner printf en backend pero nos va a servir para debugear.
    return FAILED_ON_CREATE_REQUEST;                           //despues los borro
  }

  switch (mode){
    case NAMED_PIPE:
      if(pipe(fd) != 0){
        printf("Cannot create server reponse receiver\n");
        return ERROR_CREATE_SERVER_RESPONSE_RECIEVER;
      }
      r.direction = fd[1];
      r.directionSize = sizeof(int);
      writeRequest(fd,r);
      break;
    case SOCKET:
      //TODO socket implementation
      break;
    default:
      printf("Mode not valid\n");
      return REQUEST_INVALID_TYPE;
  }

  return REQUEST_OK;
}

//TODO request queue's fd should be a global variable, we need to learn how to get an opened named pipe's fd 
void writeRequest (int fd, Request * request) {
  writeNamedPipe(fd, request -> action, sizeof(request -> action));
  writeNamedPipe(fd, request -> type, sizeof(request -> type));
  writeNamedPipe(fd, request -> dataSize, sizeof(request -> dataSize));
  writeNamedPipe(fd, request -> data, request -> dataSize);
  writeNamedPipe(fd, request -> directionSize, sizeof(request -> directionSize));
  writeNamedPipe(fd, request -> direction, request -> directionSize);
}

struct Request getRequest(int fd) {
  int aux_err;
  struct Request * rta;

  aux_err = read( fd, rta, sizeof(struct Request) );
  if ( aux_err )
    return rta;
  return NOT_FOUND_ERR; // return NULL
}

void processRequest(struct Request r) {
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
  return;
}
