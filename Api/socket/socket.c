#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include "socket.h"
#include <sys/socket.h>
#include "../commons.h"
#include <sys/un.h>

#define SOCKETPATH "/tmp/giladitaSocket"
#define LISTEN_BACKLOG 50
#define	MAX_CONNECTIONS 100

Connection * openConnection(){
	struct sockaddr_un my_addr;
	int socketfd;

	socketfd = socket(PF_LOCAL, SOCK_STREAM, 0);
	if(socketfd == -1){
		return NULL;
	}
	printf("el fd es %d\n", socketfd);
	memset(&my_addr, 0, sizeof(struct sockaddr_un));
	my_addr.sun_family = PF_LOCAL;
	strncpy(my_addr.sun_path, SOCKETPATH, sizeof(my_addr.sun_path) - 1);
	printf("el fd es %d\n", socketfd);
	if (bind(socketfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0){
		return NULL;	
	}

	printf("el fd es %d\n", socketfd);
	return createConnection(socketfd);
}


requestState writeRequest(Request * request, int fd){
  printf("START - writeRequest\n");
  printf("FD %d\n", fd);
  send(fd, &request -> action, sizeof(request -> action),0);
  send(fd, &request -> connection -> fd, sizeof(request -> connection -> fd),0);
  send(fd, &request -> connection -> dataSize, sizeof(request -> connection -> dataSize),0);
  send(fd, &request -> connection -> data, request -> connection -> dataSize,0);
  printf("END - writeRequest\n");
  return REQUEST_OK;
}

int closeSocket(int fd) {
	return shutdown(fd, SHUT_WR);
}

Request * getRequest(Connection * connection) {
  Request *request; 
  int action, fd;
  size_t dataSize;
  void * data;
  recv(connection-> fd, &action, sizeof(int),0);
  recv(connection-> fd, &fd, sizeof(int),0);
  recv(connection-> fd, &dataSize, sizeof(int),0);
  data = malloc (dataSize);
  recv(connection-> fd, data, dataSize,0);
  request = createRequest(action, fd, dataSize, data);
  printf("END - readNamedPipe\n");
  return request;
}

int requestServer(Connection * connection, int action, size_t dataSize, void * data) {
  struct sockaddr_un my_addr;
  Request * request;
  int socketfd;

  socketfd = socket (PF_LOCAL, SOCK_STREAM, 0);

  memset(&my_addr, 0, sizeof(struct sockaddr_un));
  my_addr.sun_family = PF_LOCAL;
  strncpy(my_addr.sun_path, SOCKETPATH, sizeof(my_addr.sun_path) - 1);

 if( connect(socketfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_un)) == -1){
 	return -1;
 }

  connection -> fd = socketfd;
  request = createRequest(action, socketfd, dataSize, data);

  if(request == NULL || request->connection == NULL){
    return FAILED_ON_CREATE_REQUEST;
  }

  writeRequest(request, socketfd);

  return SUCCESS;
}

int listenConnection(Connection * connection){
listen (connection -> fd, MAX_CONNECTIONS);
}

Connection * createConnection(int fd){
  Connection * connection;
  connection = malloc(sizeof(Connection));
  connection -> fd = fd;
  return connection;
}

Request * createRequest(int action, int fd, size_t dataSize, void * data){
  Connection * connection = createConnection(fd);

  Request *request = malloc(sizeof(Request));
  request -> action = action;
  request -> connection = connection;
  return request;
}







int getResponse(Connection * connection) {
  // printf("START - getResponse\n");
  // int aux_err = 0;
  // int fd = connection-> fd;
  // aux_err = readNamedPipe(fd, connection-> data);
  // closeNamedPipe(fd, REQUEST_QUEUE);
  // if ( aux_err )
  //   return ERROR;
  // connection -> dataSize = aux_err;
  // printf("END - getResponse | fd: %d\n", fd);
  return NOT_FOUND_ERR; // return NULL
}