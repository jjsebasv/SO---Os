#include "namedPipe.h" 
#include "../request.h"

int openNamedPipe(char * something) {
  char origin[] = "/tmp/";
  char myfifo[80];
  int fd;

  strcpy(myfifo,origin);
  strcat(myfifo,something);
  mkfifo(myfifo, 0666);

  // no estamos validando que open devuelva -1 no?
  // llamada recursiva?
  fd = open(myfifo, O_CREAT);

  return fd;
}

void writeNamedPipe(int fd, void * data, int size) {
  write(fd, data, size);
}

int writeRequest(Request * request) {

  writeNamedPipe(REQUEST_QUEUE, request -> action, sizeof(request -> action));
  writeNamedPipe(REQUEST_QUEUE, request -> type, sizeof(request -> type));
  writeNamedPipe(REQUEST_QUEUE, request -> dataSize, sizeof(request -> dataSize));
  writeNamedPipe(REQUEST_QUEUE, request -> data, request -> dataSize);
  writeNamedPipe(REQUEST_QUEUE, request -> directionSize, sizeof(request -> directionSize));
  writeNamedPipe(REQUEST_QUEUE, request -> direction, request -> directionSize);

  return SUCCESS;
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
int getRequest(Request * request) {
  int aux_err;
  int fd = 0;
  //TODO open pipe with its name and start reading from it

  // NAME is the named pipe name from where to read - should be a string
  fd = open(REQUEST_QUEUE, O_READONLY);
  request = (Request *)malloc(sizeof(Request));
  aux_err = read( fd, request, sizeof( Request ) );
  if ( aux_err )
    return ERROR;
  return NOT_FOUND_ERR; // return NULL
}

// getResponse defined for namedPipe
// response -> direction is, in this case, the path
int getReponse(Response * response) {
  int aux_err = 0;
  int fd = openNamedPipe(response -> direction);
  aux_err = readNamedPipe(fd, response -> response);
  closeNamedPipe( fd, response -> direction );
  if ( aux_err )
    return ERROR;
  response -> responseSize = aux_err;
  return NOT_FOUND_ERR; // return NULL
}

Connection* openConnection (Connection * connection){
  connection->connection = openNamedPipe (REQUEST_QUEUE);
  return connection; 
}
