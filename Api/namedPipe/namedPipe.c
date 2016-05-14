#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include "namedPipe.h"
#include "../commons.h"
#include "../../Database/database.h"

Connection * createConnection(int fd){
  Connection * connection;
  connection = malloc(sizeof(Connection));
  connection -> fd = fd;
  return connection;
}

int processRequest (Request * request) {
  switch (request->action) {

    case ADD_STUDENT:
      //DbAddStudent(request->connection->data->name, request->connection->data->average);
      break;

    case UPDATE_STUDENT:
      //DbUpdateStudent();
      break;

    case DELETE_STUDENT:
      //DbDeleteStudent();
      break;

    case READ_STUDENTS:
      // DbReadStudents();
      break;

    case DROP_TABLE:
      // DbDropTable();
      break;

    default:
      printf("Error request action\n");
      return -1;
  }
  return 0;
}

int * openNamedPipe(char * namedPipeName) {
  //printf("START - openNamedPipe\n");
  char origin[] = "/tmp/";
  char myfifo[80];
  int * fd;
  fd = malloc(sizeof(int)*2);
  
  strcpy(myfifo,origin);
  strcat(myfifo,namedPipeName);
  mkfifo(myfifo, 0777);
  //printf("%s\n", myfifo);

  fd[0] = open(myfifo, O_RDONLY|O_NONBLOCK);
  fcntl(fd[0], F_SETFL, fcntl(fd[0], F_GETFL) &~O_NONBLOCK);
  fd[1] = open(myfifo, O_WRONLY);

  //printf("END - openNamedPipe\n");
  return fd;
}

void writeNamedPipe(int fd, void * data, int size) {
  write(fd, data, size);
  //printf("caracteres escritos %d\n", w);
}

requestState writeRequest(Request * request, int fd) {
  //printf("START - writeRequest\n");
  //printf("FD %d\n", fd);
  printf("ACTION %d\n", request -> action);
  printf("DATA SIZE %d\n", request -> connection -> dataSize);
  printf("NAME %s\n", ((Student*)request->connection->data)-> name);
  printf("AVERAGE %s\n", ((Student*)request->connection->data)->average);
  writeNamedPipe(fd, &request -> action, sizeof(int));
  writeNamedPipe(fd, &request -> connection -> fd, sizeof(int));
  writeNamedPipe(fd, &request -> connection -> dataSize, sizeof(int));
  writeNamedPipe(fd, &request -> connection -> data, request -> connection -> dataSize);
  //printf("END - writeRequest\n");
  return REQUEST_OK;
}

int readNamedPipe (int fd, void * buffer) {
  //printf("START - readNamedPipe en fd %d\n", fd);
  buffer = malloc(BLOCK * sizeof(char));
  int q = 0;
  q = read(fd, buffer, BLOCK);
  printf("%d\n", q);
  // Se queda aca esperando a que lea del named pipe
  if (q > 0)
    buffer = (char *) realloc(buffer, q);
  //printf("END - readNamedPipe\n");
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


  // writeNamedPipe(fd, &request -> action, sizeof(request -> action));
  // writeNamedPipe(fd, &request -> connection -> fd, sizeof(request -> connection -> fd));
  // writeNamedPipe(fd, &request -> connection -> dataSize, sizeof(request -> connection -> dataSize));
  // writeNamedPipe(fd, &request -> connection -> data, request -> connection -> dataSize);

Request * getRequest(Connection * connection) {
  printf("START - getRequest\n");
  Request *request; 
  int action, fd = 0;
  printf("fd antes%d\n", fd);
  int dataSize;
  Student * student;
  read(connection-> fd, &action, sizeof(int));
  printf("Lei la action ----> %d\n", action);
  read(connection-> fd, &fd, sizeof(int));
  printf("Lei el fd ----> %d\n", fd);
  read(connection-> fd, &dataSize, sizeof(int));
  printf("Lei el datasize ----> %d\n", dataSize);
  student = malloc (dataSize);
  read(connection-> fd, student, dataSize);
  printf("Lei la data ----> name: %s average: %s\n", student->name, student->average);
  request = createRequest(action, fd, dataSize, (void*)student);
  return request;
}

int getResponse(Connection * connection) {
  //printf("START - getResponse\n");
  int aux_err = 0;
  int fd = connection-> fd;
  //printf("FD EN GET RESPONSE: %d\n", fd);
  aux_err = readNamedPipe(fd, connection-> data);
  if ( aux_err )
    return ERROR;
  connection -> dataSize = aux_err;
  //printf("END - getResponse | fd: %d\n", fd);
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
  //printf("RESPONSE responseFd[0]: %d y responseFd[1]: %d\n", responseFd[0], responseFd[1]);

  writeRequest(request, queueFd[1]);
  return SUCCESS;
}



Request * createRequest(int action, int fd, int dataSize, void * data){
  printf("dataSize%d\n", dataSize);
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
