#include "api.h"

#define PIPE 0
#define NAMED_PIPE 1
#define IP 2


int open(void * something, int type) {
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
  fd = open(myfifo, type);

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
