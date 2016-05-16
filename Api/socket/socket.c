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


static const char * serverMsg[5] = {
  "Estudiante agregado con exito ",
  "El estudiante ya existe ",
  "Estudiante modificado con exito ",
  "Estudiante eliminado con exito ",
  "Error en la base de datos "

};

Connection * openConnection(){
	struct sockaddr_un my_addr;
	int socketfd;

	socketfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(socketfd == -1){
		return NULL;
	}
	memset(&my_addr, 0, sizeof(struct sockaddr_un));
	my_addr.sun_family = AF_UNIX;
	strncpy(my_addr.sun_path, SOCKETPATH, sizeof(my_addr.sun_path) - 1);
	if (bind(socketfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0){
		return NULL;	
	}

	return createConnection(socketfd);
}


requestState writeRequest(Request * request, int fd){
  int cant;
  cant = write(fd, &request -> action, sizeof(int));
  cant = write(fd, &request -> connection -> dataSize, sizeof(int));
  cant = write(fd, request -> connection -> data, request -> connection -> dataSize);
  return REQUEST_OK;
}

int closeSocket(int fd) {
	return shutdown(fd, SHUT_WR);
}

Request * getRequest(Connection * connection, int listened) {
  Request *request; 
  int action, fd,cant;
  int dataSize;
  Student * student;
  read(listened, &action, sizeof(int));
  read(listened, &dataSize, sizeof(int));
  student = malloc (dataSize);
  read(listened, student, dataSize);
  request = createRequest(action, listened, dataSize, (void*)student);
  return request;
}

int requestServer(Connection * connection, int action, int dataSize, void * data) {
  struct sockaddr_un my_addr;
  Request * request;
  int socketfd;

  socketfd = socket (AF_UNIX, SOCK_STREAM, 0);

  memset(&my_addr, 0, sizeof(struct sockaddr_un));
  my_addr.sun_family = AF_UNIX;
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
struct sockaddr_un clientName;
socklen_t clientNameLen;
int clientSocketfd;
listen (connection -> fd, MAX_CONNECTIONS);
clientSocketfd = accept(connection -> fd, (struct sockaddr*)&clientName, &clientNameLen);
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
  int aux_err = 0;
  char * buffer;
  int fd = connection-> fd, size;
  printf("%d\n",connection ->fd );
  aux_err = read(fd, &size, sizeof(int));
  if ( aux_err == -1){
  	closeSocket(fd);
  	return ERROR;
  }
  buffer = malloc(size+1);
  aux_err = read(fd,buffer , size+1);

  if ( aux_err ==-1){
    return ERROR;
    
  }
  printf("END - getResponse | fd: %d\n response: %s\n", fd,buffer);
  return NOT_FOUND_ERR; // return NULL
}

int writeResponse (Request * request, int state) {
  int dataSize = strlen(serverMsg[state]+1);
  int written = write(request->connection->fd, &dataSize, sizeof(dataSize));
  written += write(request->connection->fd, serverMsg[state], dataSize);
  closeSocket(request->connection->fd);
  return 0;
}