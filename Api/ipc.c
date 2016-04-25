#include "api.h"

#define BLOCK 1024

int open(void * something, connectionType type) {
  switch (type) {
    case PIPE :
      return openPipe(something);
      break;
    case NAMED_PIPE :
      return openNamedPipe(something);
      break;
    case IP :
      return openSocket(something);
      break;
  }
  return -1;
}

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
