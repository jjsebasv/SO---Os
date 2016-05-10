#include <stdlib.h>
#include "namedPipe.h" 
#include "../request.h"

int * openNamedPipe(char * something) {
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
  return fd;
}

void writeNamedPipe(int fd, void * data, int size) {
  write(fd, data, size);
}

requestState writeRequest(Request * request, int fd) {
  writeNamedPipe(fd, &request -> action, sizeof(request -> action));
  writeNamedPipe(fd, &request -> connection -> np -> fd, sizeof(request -> connection -> np -> fd));
  writeNamedPipe(fd, &request -> connection -> np -> dataSize, sizeof(request -> connection -> np -> dataSize));
  writeNamedPipe(fd, &request -> connection -> np -> data, request -> connection -> np -> dataSize);
  return REQUEST_OK;
}

int readNamedPipe (int fd, char * buffer) {
  buffer = malloc(BLOCK * sizeof(char));
  int q = 0;
  q = read(fd, buffer, BLOCK);
  if (q > 0)
    buffer = (char *) realloc(buffer, q);
  return q;
}

int closeNamedPipe(int fd, char * something) {
  char origin[] = "/tmp/";
  char * myfifo = strcat(origin,something);

  close(fd);
  unlink(myfifo);

  return 0;
}

//TODO We should redo this function. it doesn't work for structures of different sizes
// Request * getRequest(Connection * connection) {
//   int aux_err;
//   int fd = 0;
//   //TODO open pipe with its name and start reading from it

//   // NAME is the named pipe name from where to read - should be a string
//   fd = open(REQUEST_QUEUE, O_READONLY);
//   request = (Request *)malloc(sizeof(Request));
//   aux_err = read( fd, request, sizeof( Request ) );
//   if ( aux_err )
//     return ERROR;
//   return NOT_FOUND_ERR; // return NULL
// }

// getResponse defined for namedPipe
// response -> direction is, in this case, the path
int getResponse(Connection * connection) {
  int aux_err = 0;
  int fd = connection->np->fd;
  aux_err = readNamedPipe(fd, connection -> np->data);
  closeNamedPipe(fd, REQUEST_QUEUE);
  if ( aux_err )
    return ERROR;
  connection -> np -> dataSize = aux_err;
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

  if(request == NULL){
    return FAILED_ON_CREATE_REQUEST;                          
  }
  
  if(c == NULL){
    return ERROR_OPEN_REQUEST_QUEUE;
  }
  connection = c;

  NPfd = openNamedPipe(REQUEST_QUEUE); //TODO check failure
  writeRequest(request, NPfd[1]);

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

//TODO malloc check fails
Connection * createConnection(int fd){
  Connection * connection;
  
  connection = malloc(sizeof(Connection));
  connection -> np = malloc(sizeof(NPConnection));

  connection -> np -> fd = fd;

  return connection;
}

  void monitorConnection(Connection * connection, fd_set* set){
    FD_ZERO(set);
    FD_SET(connection -> np -> fd, set);
    return;
  }
