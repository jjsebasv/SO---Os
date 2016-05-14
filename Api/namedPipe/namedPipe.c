#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include "namedPipe.h"
#include "../commons.h"
#include <signal.h>
#include "../../Database/databaseapi.h"

Connection * createConnection(int fd){
  Connection * connection;
  connection = malloc(sizeof(Connection));
  connection -> fd = fd;
  return connection;
}

void processRequestServer (Request * request) {
  int pid = fork();
  if (pid == 0) {
    // Server - slave
    int* queueFd;
    queueFd = openNamedPipe(SQL_QUEUE);
    writeRequest(request, queueFd[1]);
    kill(getpid(), SIGKILL);
  } else if (pid > 0) {
    // Do nothing 
    // Server - master
  } else {
    // fork error
  }
}

void processRequestDatabase (Request * request) {
  int state;
  switch (request->action) {

    case ADD_STUDENT:
      state = DbAddStudent(request->connection->data->name, request->connection->data->average);
      printf("STATE %d\n", state);
      write(request->connection->fd, &state, sizeof(int));
      break;

    case UPDATE_STUDENT:
      //DbUpdateStudent();
      break;

    case DELETE_STUDENT:
      DbDeleteStudent(request->connection->data->name);
      break;

    case READ_STUDENTS:
      DbReadStudents();
      break;

    case DROP_TABLE:
      DbDropTable();
      break;

    case CREATE_TABLE:
      DbCreateTable();
      break;

    default:
      printf("Error request action\n");
  }
}

int * openNamedPipe(char * namedPipeName) {
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
<<<<<<< HEAD
  student = malloc (dataSize);
  read(connection-> fd, student, dataSize);
  request = createRequest(action, fd, dataSize, (void*)student);
=======
  data = malloc (dataSize);
  read(connection-> fd, data, dataSize);
  request = createRequest(action, fd, dataSize, data);
  printf("END - readNamedPipe\n");
>>>>>>> fix
  return request;
}

int getResponse(Connection * connection) {
  int fd = connection-> fd;
  int answer;
  read(fd, &answer, sizeof(int));
  return answer;
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
  //printf("RESPONSE responseFd[0]: %d y responseFd[1]: %d\n", responseFd[0], responseFd[1]);

  writeRequest(request, queueFd[1]);
  return SUCCESS;
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
