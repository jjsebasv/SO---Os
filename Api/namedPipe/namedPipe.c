#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include "namedPipe.h"
#include "../commons.h"
#include <signal.h>
#include <string.h>
#include <sys/uio.h>
#include "../../Server/databaseapi.h"

static const char * serverMsg[5] = {
  "Estudiante agregado con exito",
  "El estudiante ya existe",
  "Estudiante modificado con exito",
  "Estudiante eliminado con exito",
  "Error en la base de datos"
};

Connection * createConnection(int fd){
  Connection * connection;
  connection = malloc(sizeof(Connection));
  connection -> fd = fd;
  return connection;
}

int * openNamedPipe(char * namedPipeName) {
  char origin[] = "/tmp/";
  char myfifo[80];
  int * fd;
  fd = malloc(sizeof(int)*2);
  
  strcpy(myfifo,origin);
  strcat(myfifo,namedPipeName);
  mkfifo(myfifo, 0777);

  fd[0] = open(myfifo, O_RDONLY|O_NONBLOCK);
  fcntl(fd[0], F_SETFL, fcntl(fd[0], F_GETFL) &~O_NONBLOCK);
  fd[1] = open(myfifo, O_WRONLY);

  return fd;
}

void writeNamedPipe(int fd, void * data, int size) {
  write(fd, data, size);
}

requestState writeRequest(Request * request, int fd) {
  writeNamedPipe(fd, &request -> action, sizeof(int));
  writeNamedPipe(fd, &request -> connection -> fd, sizeof(int));
  writeNamedPipe(fd, &request -> connection -> dataSize, sizeof(int));
  writeNamedPipe(fd, request -> connection -> data, request -> connection -> dataSize);
  return REQUEST_OK;
}

int readNamedPipe (int fd, void * buffer) {
  buffer = malloc(BLOCK * sizeof(char));
  int q = 0;
  q = read(fd, buffer, BLOCK);
  printf("%d\n", q);
  if (q > 0)
    buffer = (char *) realloc(buffer, q);
  return q;
}

int closeNamedPipe(int fd, char * name) {
  char origin[] = "/tmp/";
  char myfifo[80] = "";
  strcat(myfifo, origin);
  strcat(myfifo, name);

  printf("%s\n", myfifo);

  close(fd);
  unlink(myfifo);

  return 0;
}

Request * getRequest(Connection * connection) {
  Request *request; 
  int action, fd = 0;
  int dataSize;
  Student * student;
  read(connection-> fd, &action, sizeof(int));
  read(connection-> fd, &fd, sizeof(int));
  read(connection-> fd, &dataSize, sizeof(int));
  student = malloc (dataSize);
  read(connection-> fd, student, dataSize);
  request = createRequest(action, fd, dataSize, (void*)student);
  return request;
}

int getResponse(Connection * connection) {
  printf("vamo a leer de %d\n", connection-> fd);
  int fd = connection-> fd;
  int r = 0;
  int nread = 0;
  int size = BLOCK;
  char * readBuffer = malloc(BLOCK);
  char * tmp;
 
  while ( (r = read(fd, readBuffer + nread, BLOCK))  ) {
    if (r > 0) {
      nread += r;
    }

    // realloc
    if ( size <= nread ) {
      if ( !(tmp = realloc(readBuffer, size + BLOCK)) ) {
        readBuffer = tmp;
      } else {
        printf("Error when reading response\n");
        return 0;
      }
    }
  }

  printf("%s\n", readBuffer);
  return nread;
}

Connection* openConnection (char * namedPipe){
  Connection * connection;
  int* fd = openNamedPipe(namedPipe);
  // change here to set where the server reads ******
  connection = createConnection(fd[0]);
  return connection;
}

int requestServer(Connection * connection, int action, int dataSize, void * data) {
  Request * request;
  int responseFd[2];
  int* queueFd;
  queueFd = openNamedPipe(REQUEST_QUEUE);

  if( pipe(responseFd) != 0){
    return -1;
  }

  request = createRequest(action, responseFd[1], dataSize, data);
  *connection = (*request->connection);


  if(request == NULL || request->connection == NULL){
    return FAILED_ON_CREATE_REQUEST;
  }
  //printf("QUEUE queueFd[0]: %d y queueFd[1]: %d\n", queueFd[0], queueFd[1]);
  // responseFd[0] leer responseFd[1] escribir
  printf("RESPONSE responseFd[0]: %d y responseFd[1]: %d\n", responseFd[0], responseFd[1]);

  writeRequest(request, queueFd[1]);
  return responseFd[0];
}



Request * createRequest(int action, int fd, int dataSize, void * data){
  Connection * connection = createConnection(fd);
  connection -> dataSize = dataSize;
  connection -> data = malloc(dataSize);
  connection -> data = data;
  Request *request = malloc(sizeof(Request));
  request -> action = action;
  request -> connection = malloc(sizeof(connection));
  request -> connection = connection;
  return request;
}

void monitorConnection(Connection * connection, fd_set* set){
  FD_ZERO(set);
  FD_SET(connection -> fd, set);
  return;
}
