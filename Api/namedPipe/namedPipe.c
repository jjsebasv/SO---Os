int openNamedPipe(char * something) {
  char[130] origin = '/tmp/';

  int fd;
  char * myfifo = strcat(origin,something);

  mkfifo(myfifo, 0666);

  // no estamos validando que open devuelva -1 no? 
  // llamada recursiva?
  fd = open(myfifo, NAMED_PIPE);

  return fd;
}

void writeNamedPipe(int fd, void * data, int size) {
  write(fd, data, size);
}

int writeRequest (Request * request) {

  writeNamedPipe(NAMED_PIPE_QUEUE, request -> action, sizeof(request -> action));
  writeNamedPipe(NAMED_PIPE_QUEUE, request -> type, sizeof(request -> type));
  writeNamedPipe(NAMED_PIPE_QUEUE, request -> dataSize, sizeof(request -> dataSize));
  writeNamedPipe(NAMED_PIPE_QUEUE, request -> data, request -> dataSize);
  writeNamedPipe(NAMED_PIPE_QUEUE, request -> directionSize, sizeof(request -> directionSize));
  writeNamedPipe(NAMED_PIPE_QUEUE, request -> direction, request -> directionSize);
}

void readNamedPipe (int fd) {
  char writeBuffer[BLOCK];
  
  while ( q > 0 ) {
    q = read(fd, writeBuffer, BLOCK);
    printf('%s', writeBuffer);
  }
}

int closeNamedPipe(int fd, char * something) {
  char[130] origin = '/tmp/';
  char * myfifo = strcat(origin,something);

  close(fd);
  unlink(myfifo);

  return 0;
}
