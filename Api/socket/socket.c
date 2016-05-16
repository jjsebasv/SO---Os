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

	socketfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(socketfd == -1){
		return NULL;
	}
	printf("el fd es %d\n", socketfd);
	memset(&my_addr, 0, sizeof(struct sockaddr_un));
	my_addr.sun_family = AF_UNIX;
	strncpy(my_addr.sun_path, SOCKETPATH, sizeof(my_addr.sun_path) - 1);
	printf("el fd es %d\n", socketfd);
	if (bind(socketfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0){
		return NULL;	
	}

	printf("el fd es %d\n", socketfd);
	return createConnection(socketfd);
}


requestState writeRequest(Request * request, int fd){
  int cant;
  printf("START - writeRequest\n");
  printf("FD %d\n", fd);
  cant = write(fd, &request -> action, sizeof(request -> action));
  printf("cantidad de caracteres escritos : %d\n",cant);
  cant = write(fd, &request -> connection -> fd, sizeof(request -> connection -> fd));
  printf("cantidad de caracteres escritos : %d\n",cant);
  printf("size = %d\n",request->connection->dataSize);
  cant = write(fd, &request -> connection -> dataSize, sizeof(request -> connection -> dataSize));
  printf("cantidad de caracteres escritos : %d\n",cant);
  cant = write(fd, &request -> connection -> data, request -> connection -> dataSize);
  printf("cantidad de caracteres escritos : %d\n",cant);
  printf("END - writeRequest\n");
  return REQUEST_OK;
}

int closeSocket(int fd) {
	return shutdown(fd, SHUT_WR);
}

Request * getRequest(Connection * connection, int listened) {
  Request *request; 
  int action, fd,cant;
  int dataSize;
  void * data;
  printf("fd asdasd %d\n",connection ->fd);
  printf("numero del fd del cliente: %d\n",listened);
  cant = read(listened, &action, sizeof(int));
  printf("cantidad de caracteres leidos: %d\n",cant);
  cant = read(listened, &fd, sizeof(int));
  printf("cantidad de caracteres leidos: %d\n",cant);
  cant = read(listened, &dataSize, sizeof(int));
  printf("cantidad de caracteres leidos: %d\n",cant);
  data = malloc (dataSize);
  cant = read(listened, data, dataSize);
  printf("cantidad de caracteres leidos: %d\n",cant);
  request = createRequest(action, listened, dataSize, data);
  printf("END - readNamedPipe\n");
  return request;
}

int requestServer(Connection * connection, int action, int dataSize, void * data) {
  struct sockaddr_un my_addr;
  Request * request;
  int socketfd;

  socketfd = socket (AF_UNIX, SOCK_STREAM, 0);

  printf("data size %d\n",dataSize);
  memset(&my_addr, 0, sizeof(struct sockaddr_un));
  my_addr.sun_family = AF_UNIX;
  strncpy(my_addr.sun_path, SOCKETPATH, sizeof(my_addr.sun_path) - 1);

 if( connect(socketfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_un)) == -1){
 	return -1;
 }

  connection -> fd = socketfd;
  request = createRequest(action, socketfd, dataSize, data);
  printf("data size %d\n",request->connection->dataSize);
  if(request == NULL || request->connection == NULL){
    return FAILED_ON_CREATE_REQUEST;
  }

  writeRequest(request, socketfd);

  return SUCCESS;
}

int listenConnection(Connection * connection){
struct sockaddr_un clientName;
socklen_t clientNameLen;
int clientSocketfd;
listen (connection -> fd, MAX_CONNECTIONS);
clientSocketfd = accept(connection -> fd, (struct sockaddr*)&clientName, &clientNameLen);
printf("el socket del cliente es: %d\n",clientSocketfd );
return clientSocketfd;
}

Connection * createConnection(int fd){
  Connection * connection;
  connection = malloc(sizeof(Connection));
  connection -> fd = fd;
  return connection;
}

Request * createRequest(int action, int fd, int dataSize, void * data){
  Connection * connection = createConnection(fd);

  Request *request = malloc(sizeof(Request));
  request -> action = action;
  request -> connection = connection;
  request ->connection -> dataSize = dataSize;
  request -> connection -> data = data;
  return request;
}

int getResponse(Connection * connection) {
  printf("START - getResponse\n");
  int aux_err = 0;
  int fd = connection-> fd, size;
  aux_err = recv(fd, &size, sizeof(int),0);
  if ( aux_err ){
  	closeSocket(fd);
  	return ERROR;
  }
  connection -> dataSize = size;
  connection -> data = malloc(size);
  aux_err = recv(fd, &connection-> data, size,0);
  if ( aux_err )
    return ERROR;
  printf("END - getResponse | fd: %d\n", fd);
  return NOT_FOUND_ERR; // return NULL
}