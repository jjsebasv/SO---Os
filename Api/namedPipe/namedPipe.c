#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include "namedPipe.h" 
#include "../request.h"

Connection * createConnection(int fd){
  printf("START - createConnection | fd: %d\n", fd);
  Connection * connection;
  
  connection = malloc(sizeof(Connection));
  connection -> np = malloc(sizeof(NPConnection));

  connection -> np -> fd = fd;

  printf("END - createConnection\n");
  return connection;
}

int * openNamedPipe(char * something) {
  printf("START - openNamedPipe\n");
  char origin[] = "/tmp/";
  char myfifo[80];
  int * fd;
  fd = malloc(sizeof(int)*2);
  
  strcpy(myfifo,origin);
  strcat(myfifo,something);
  mkfifo(myfifo, 0666);

  fd[0] = open(myfifo, O_RDONLY|O_NONBLOCK);
  fcntl(fd[0], F_SETFL, fcntl(fd[0], F_GETFL) &~O_NONBLOCK);
  fd[1] = open(myfifo, O_WRONLY);
  printf("END - openNamedPipe\n");
  return fd;
}

void writeNamedPipe(int fd, void * data, int size) {
  write(fd, data, size);
}

requestState writeRequest(Request * request, int fd) {
  printf("START - writeRequest\n");
  printf("FD %d\n", fd);
  writeNamedPipe(fd, &request -> action, sizeof(request -> action));
  writeNamedPipe(fd, &request -> connection -> np -> fd, sizeof(request -> connection -> np -> fd));
  writeNamedPipe(fd, &request -> connection -> np -> dataSize, sizeof(request -> connection -> np -> dataSize));
  writeNamedPipe(fd, &request -> connection -> np -> data, request -> connection -> np -> dataSize);
  printf("END - writeRequest\n");
  return REQUEST_OK;
}

int readNamedPipe (int fd, char * buffer) {
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
  char * myfifo = strcat(origin,something);

  close(fd);
  unlink(myfifo);

  printf("END - closeNamedPipe\n");
  return 0;
}

Request * getRequest(Connection * connection) {
  printf("START - getRequest\n");
  int aux_err;
  int fd = connection -> np -> fd;
  Request *request = malloc(sizeof(Request));
  aux_err = read( fd, request, sizeof( Request ) );
  printf("aux_err\n");

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
  Connection * connection;
  int* fd = openNamedPipe (REQUEST_QUEUE);
  connection = createConnection(fd[0]);
  return connection; 
}

//TODO CHECK LATER: FIRST ARGUMENT SHOULD IT BE Connection **?
int requestServer(Connection * connection, int action, size_t dataSize, void * data) {
  printf("START - Request server\n");
  Request * request;
  Connection * c;
  // REDO WITHOUT NAME PIPES
  int fd[2];
  int* NPfd;

  if( pipe(fd) != 0){
    return -1;
  }

  printf("fd[0]: %d y fd[1]: %d\n", fd[0], fd[1]);
  request = createRequest(action, fd[1], dataSize, data);
  c = createConnection(fd[0]);

  if(request == NULL){
    return FAILED_ON_CREATE_REQUEST;                          
  }
  
  if(c == NULL){
    return ERROR_OPEN_REQUEST_QUEUE;
  }

  *connection = *c;

  NPfd = openNamedPipe(REQUEST_QUEUE); //TODO check failure
  printf("NPfd[0]: %d y NPfd[1]: %d\n", NPfd[0], NPfd[1]);
  writeRequest(request, NPfd[1]);

  printf("END - requestServer\n");
  return SUCCESS; 
}


//TODO malloc check fails
Request * createRequest(int action, int fd, size_t dataSize, void * data){
  Request * request;

  request = malloc(sizeof(Request));
  request -> connection = malloc (sizeof(Connection));
  request -> connection -> np = malloc (sizeof(NPConnection)); 

  request -> action = action;
  request -> connection -> np -> fd = fd;
  request -> connection -> np -> dataSize = dataSize;
  request -> connection -> np -> data = data;

  return request;
}

void monitorConnection(Connection * connection, fd_set* set){
  FD_ZERO(set);
  FD_SET(connection -> np -> fd, set);
  return;
}
