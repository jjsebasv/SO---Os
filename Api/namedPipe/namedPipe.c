#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include "namedPipe.h"
#include "../request.h"

Connection * createConnection(int fd){
  //printf("START - createConnection\n");
  Connection * connection;
  connection = malloc(sizeof(Connection));
  connection -> np = malloc(sizeof(NPConnection));
  connection -> np -> fd = fd;
  //printf("END - createConnection\n");
  return connection;
}

/**
 *  Returns two fds for the namedPipeName
 *
 *  @param  namedPipeName name of the pipe
 */
int * openNamedPipe(char * namedPipeName) {
  //printf("START - openNamedPipe\n");
  char origin[] = "/tmp/";
  char myfifo[80];
  int * fd;
  fd = malloc(sizeof(int)*2);
  
  strcpy(myfifo,origin);
  strcat(myfifo,namedPipeName);
  mkfifo(myfifo, 0777);

  fd[0] = open(myfifo, O_RDONLY|O_NONBLOCK);
  fd[1] = open(myfifo, O_WRONLY);

  //printf("END - openNamedPipe\n");
  return fd;
}

void writeNamedPipe(int fd, void * data, int size) {
  int w = write(fd, data, size);
  printf("caracteres escritos %d\n", w);
}

requestState writeRequest(Request * request, int fd) {
  printf("START - writeRequest\n");
  printf("FD %d\n", fd);
  printf("size of request %lu \n", sizeof((*request)));
  // writeNamedPipe(fd, &request -> action, sizeof(request -> action));
  // writeNamedPipe(fd, &request -> connection -> np -> fd, sizeof(request -> connection -> np -> fd));
  // writeNamedPipe(fd, &request -> connection -> np -> dataSize, sizeof(request -> connection -> np -> dataSize));
  // writeNamedPipe(fd, &request -> connection -> np -> data, request -> connection -> np -> dataSize);
  writeNamedPipe(fd, request, sizeof((*request)));
  printf("END - writeRequest\n");
  return REQUEST_OK;
}

int readNamedPipe (int fd, void * buffer) {
  printf("START - readNamedPipe\n");
  buffer = malloc(BLOCK * sizeof(char));
  int q = 0;
  q = read(fd, buffer, BLOCK);
  // Se queda aca esperando a que lea del named pipe
  if (q > 0)
    buffer = (char *) realloc(buffer, q);
  printf("END - readNamedPipe\n");
  return q;
}

int closeNamedPipe(int fd, char * something) {
  printf("START - closeNamedPipe\n");
  char origin[] = "/tmp/";
  char myfifo[80] = "";
  strcat(myfifo, origin);
  strcat(myfifo, something);

  printf("%s\n", myfifo);

  close(fd);
  unlink(myfifo);

  printf("END - closeNamedPipe\n");
  return 0;
}

Request * getRequest(Connection * connection) {
  printf("START - getRequest\n");
  int aux_err;
  //int fd = 0;
  //fd = fopen(REQUEST_QUEUE, O_READONLY);
  printf("EL FD!%d\n", connection -> np-> fd);
  Request *request = malloc(sizeof(Request));
  aux_err = readNamedPipe(connection -> np-> fd, request);
  // aux_err = read( fd, request, sizeof( Request ) );`
  printf("CARACTERES LEIDOS %d\n", aux_err);
  printf("Vamo a ver la request\n");
  printf("request action %d\n", request->action);
  printf("connection fd %d\n", request->connection->np->fd);
  printf("connection dataSize %d\n", request->connection->np->dataSize);

  // if ( aux_err )
  //   return ERROR;
  // return NOT_FOUND_ERR; // return NULL
  printf("END - getRequest\n");
  return request;
}

int getResponse(Connection * connection) {
  printf("START - getResponse\n");
  int aux_err = 0;
  int fd = connection->np->fd;
  aux_err = readNamedPipe(fd, connection -> np->data);
  closeNamedPipe(fd, REQUEST_QUEUE);
  if ( aux_err )
    return ERROR;
  connection -> np -> dataSize = aux_err;
  printf("END - getResponse | fd: %d\n", fd);
  return NOT_FOUND_ERR; // return NULL
}

Connection* openConnection (void){
  printf("START - openConnection\n");
  Connection * connection;
  int* fd = openNamedPipe(REQUEST_QUEUE);
  // change here to set where the server reads ******
  connection = createConnection(fd[0]);
  printf("END - openConnection\n");
  return connection;
}

int requestServer(Connection * connection, int action, size_t dataSize, void * data) {
  Request * request;
  int responseFd[2];
  int* queueFd;

  if( pipe(responseFd) != 0){
    return -1;
  }

  printf("responseFd[0]: %d y responseFd[1]: %d\n", responseFd[0], responseFd[1]);
  connection = createConnection(responseFd[1]);
  request = createRequest(action, responseFd[1], dataSize, data);


  queueFd = openNamedPipe(REQUEST_QUEUE);

  if(request == NULL || request->connection == NULL){
    return FAILED_ON_CREATE_REQUEST;
  }
  printf("QUEUE queueFd[0]: %d y queueFd[1]: %d\n", queueFd[0], queueFd[1]);
  printf("RESPONSE responseFd[0]: %d y responseFd[1]: %d\n", responseFd[0], responseFd[1]);

  writeRequest(request, queueFd[1]);
  closeNamedPipe(queueFd[1],  REQUEST_QUEUE);
  //printf("END - requestServer\n");
  return SUCCESS;
}



Request * createRequest(int action, int fd, size_t dataSize, void * data){
  Connection * connection = createConnection(fd);

  Request *request = malloc(sizeof(Request));
  request -> action = action;
  request -> connection = connection;
  return request;
}

void monitorConnection(Connection * connection, fd_set* set){
  FD_ZERO(set);
  FD_SET(connection -> np -> fd, set);
  return;
}
