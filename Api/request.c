#include "ipc.h"
#include "request.h"

void myRequest() {
  int fd[2];
  int myPipe = pipe(fd);
  createRequest();
  sendRequest();
}


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

// action = 0 read
// action = 1 write
void processRequest(struct Request r) {
  switch (r -> action) {
    case 0:
      return readRequest(r);
      break;
    case 1:
      return writeRequest(r);
      break;
  }
  return;
}



// TODO
// #define NOT_FOUND_ERR
// int processRequest(struct Request r) {};
// readRequest(r)
// writeRequest(r)