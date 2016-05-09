  #include "namedPipe.h" 

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

void writeRequest (Request * request) {

  writeNamedPipe(NAMED_PIPE_QUEUE, request -> action, sizeof(request -> action));
  writeNamedPipe(NAMED_PIPE_QUEUE, request -> type, sizeof(request -> type));
  writeNamedPipe(NAMED_PIPE_QUEUE, request -> dataSize, sizeof(request -> dataSize));
  writeNamedPipe(NAMED_PIPE_QUEUE, request -> data, request -> dataSize);
  writeNamedPipe(NAMED_PIPE_QUEUE, request -> directionSize, sizeof(request -> directionSize));
  writeNamedPipe(NAMED_PIPE_QUEUE, request -> direction, request -> directionSize);
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
