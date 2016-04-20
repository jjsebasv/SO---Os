#include "IPC.h"


void myRequest() {
  int fd[2];
  int myPipe = pipe(fd);
  createRequest();
  sendRequest();
}