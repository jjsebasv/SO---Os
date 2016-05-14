#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include "namedPipe.h"
#include "../commons.h"

Connection * createConnection(int fd){
  Connection * connection;
  connection = malloc(sizeof(Connection));
  connection -> fd = fd;
  return connection;
}


int * openNamedPipe(char * namedPipeName) {
  //printf("START - openNamedPipe\n");
  char origin[] = "/tmp/";
  char myfifo[80];
  int * fd;
  fd = malloc(sizeof(int)*2);
  
  strcpy(myfifo,origin);
  strcat(myfifo,namedPipeName);
  mkfifo(myfifo, 0666);

  fd[0] = open(myfifo, O_RDONLY|O_NONBLOCK);
  fcntl(fd[0], F_SETFL, fcntl(fd[0], F_GETFL) &~O_NONBLOCK);
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
  writeNamedPipe(fd, &request -> action, sizeof(request -> action));
  writeNamedPipe(fd, &request -> connection -> fd, sizeof(request -> connection -> fd));
  writeNamedPipe(fd, &request -> connection -> dataSize, sizeof(request -> connection -> dataSize));
  writeNamedPipe(fd, &request -> connection -> data, request -> connection -> dataSize);
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

int closeNamedPipe(int fd, char * name) {
  printf("START - closeNamedPipe\n");
  char origin[] = "/tmp/";
  char myfifo[80] = "";
  strcat(myfifo, origin);
  strcat(myfifo, name);

  printf("%s\n", myfifo);

  close(fd);
  unlink(myfifo);

  printf("END - closeNamedPipe\n");
  return 0;
}

Request * getRequest(Connection * connection) {
  printf("START - getRequest\n");
  Request *request; 
  int action, fd;
  size_t dataSize;
  void * data;
  printf("START - readNamedPipe\n");
  read(connection-> fd, &action, sizeof(int));
  read(connection-> fd, &fd, sizeof(int));
  read(connection-> fd, &dataSize, sizeof(int));
  data = malloc (dataSize);
  read(connection-> fd, data, dataSize);
  request = createRequest(action, fd, dataSize, data);
  printf("END - readNamedPipe\n");
  return request;
}

int getResponse(Connection * connection) {
  printf("START - getResponse\n");
  int aux_err = 0;
  int fd = connection-> fd;
  aux_err = readNamedPipe(fd, connection-> data);
  closeNamedPipe(fd, REQUEST_QUEUE);
  if ( aux_err )
    return ERROR;
  connection -> dataSize = aux_err;
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

  connection -> fd = responseFd[0];
  request = createRequest(action, responseFd[1], dataSize, data);


  queueFd = openNamedPipe(REQUEST_QUEUE);

  if(request == NULL || request->connection == NULL){
    return FAILED_ON_CREATE_REQUEST;
  }
  printf("QUEUE queueFd[0]: %d y queueFd[1]: %d\n", queueFd[0], queueFd[1]);
  printf("RESPONSE responseFd[0]: %d y responseFd[1]: %d\n", responseFd[0], responseFd[1]);

  writeRequest(request, queueFd[1]);
  // closeNamedPipe(queueFd[1],  REQUEST_QUEUE);
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
  FD_SET(connection -> fd, set);
  return;
}

int listenConnection(Connection * connection){
    fd_set set;
    monitorConnection(connection, &set);
    return fdCount = select(connection -> fd + 1, &set, NULL, NULL, NULL);
}