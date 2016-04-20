// From http://stackoverflow.com/questions/2784500/how-to-send-a-simple-string-between-two-programs-using-pipes
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// O_WRONLY = 1
// R_WRONLY = 0

#include "api.h"

#define PIPE 0
#define NAMED_PIPE 1
#define IP 2

#define SERVER_SERVER "tmp/server"
#define CLIENT_SERVER "tmp/client"

static const int server_server_fd;

int open(void * something, int wut) {
  switch (wut) {
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

void myRequest() {
  int fd[2];
  int myPipe = pipe(fd);
  createRequest();
  sendRequest();
}


// Private

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

void readNamedPipe(int fd) {

}

int closeNamedPipe(int fd, char * something) {
  char[130] origin = '/tmp/';
  char * myfifo = strcat(origin,something);

  close(fd);
  unlink(myfifo);

  return 0;
}
