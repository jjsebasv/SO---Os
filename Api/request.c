#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include "request.h"

#define NOT_FOUND ((void*)0)

void sendRequest(Request * request){
  printf("Send request:\n");
  // to do
  // writeRequestQueue namedPipe.c
  // writeRequestQueue socket.c
  // writeRequestQueue(request);
}

// TO DO 
void createConnection(Connection * connection) {
  return;
}

int getConnection (Connection * connection) {
  return 0;
}


