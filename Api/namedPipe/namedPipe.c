#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include "namedPipe.h" 
#include "../request.h"

Connection * createConnection(int fd){
  printf("create connection\n");
  printf("fd %d\n", fd);
  Connection * connection;
  
  connection = malloc(sizeof(Connection));
  connection -> np = malloc(sizeof(NPConnection));

  connection -> np -> fd = fd;

  return connection;
}

int * openNamedPipe(char * something) {
  printf("openNamedPipe\n");
  char origin[] = "/tmp/";
  char myfifo[80];
  int * fd;
  fd = malloc(sizeof(int)*2);
  
  strcpy(myfifo,origin);
  strcat(myfifo,something);
  mkfifo(myfifo, 0666);

  // no estamos validando que open devuelva -1 no?
  // llamada recursiva?
  fd[0] = open(myfifo, O_RDONLY|O_NONBLOCK);
  fcntl(fd[0], F_SETFL, fcntl(fd[0], F_GETFL) &~O_NONBLOCK);
  fd[1] = open(myfifo, O_WRONLY);
  printf("Me voy de openNamedPipe\n");
  return fd;
}

void writeNamedPipe(int fd, void * data, int size) {
  write(fd, data, size);
}

requestState writeRequest(Request * request, int fd) {

  printf("writeRequest\n");
  writeNamedPipe(fd, &request -> action, sizeof(request -> action));
  writeNamedPipe(fd, &request -> connection -> np -> fd, sizeof(request -> connection -> np -> fd));
  writeNamedPipe(fd, &request -> connection -> np -> dataSize, sizeof(request -> connection -> np -> dataSize));
  writeNamedPipe(fd, &request -> connection -> np -> data, request -> connection -> np -> dataSize);
  printf("me voy de writeRequest\n");
  return REQUEST_OK;
}

int readNamedPipe (int fd, char * buffer) {
  printf("ReadNamedPipe\n");
  buffer = malloc(BLOCK * sizeof(char));
  int q = 0;
  q = read(fd, buffer, BLOCK);
  if (q > 0)
    buffer = (char *) realloc(buffer, q);
  return q;
}

int closeNamedPipe(int fd, char * something) {
  printf("closeNamedPipe\n");
  char origin[] = "/tmp/";
  char * myfifo = strcat(origin,something);

  close(fd);
  unlink(myfifo);

  return 0;
}

//TODO We should redo this function. it doesn't work for structures of different sizes
Request * getRequest(Connection * connection) {
  int aux_err, action, fd;
  size_t dataSize;
  void * data;
  Request * request;

  aux_err = read( connection -> np -> fd, request, sizeof( Request ) );

  request = createRequest (action, fd, dataSize, data);
  aux_err = read( connection -> np -> fd, request, sizeof( Request ) );
  if ( aux_err )
    return ERROR;
  return NOT_FOUND_ERR; // return NULL
}


int getResponse(Connection * connection) {
  printf("getResponse\n");
  int aux_err = 0;
  int fd = connection->np->fd;
  printf("fd: %d\n", fd);
  aux_err = readNamedPipe(fd, connection -> np->data);
  closeNamedPipe(fd, REQUEST_QUEUE);
  if ( aux_err )
    return ERROR;
  connection -> np -> dataSize = aux_err;
  printf("me voy de getResponse\n");
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
  printf("Request server\n");
  Request * request;
  Connection * c;
  // REDO WITHOUT NAME PIPES
  int fd[2];
  int* NPfd;

  if( pipe(fd) != 0){
    return -1;
  }

  request = createRequest(action, fd[1], dataSize, data);
  c = createConnection(fd[0]);
  printf("requestServer fd: %d\n", c->np->fd);

  if(request == NULL){
    return FAILED_ON_CREATE_REQUEST;                          
  }
  
  if(c == NULL){
    return ERROR_OPEN_REQUEST_QUEUE;
  }

  *connection = *c;

  NPfd = openNamedPipe(REQUEST_QUEUE); //TODO check failure
  writeRequest(request, NPfd[1]);

  printf("Me voy de requestServer\n");
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
