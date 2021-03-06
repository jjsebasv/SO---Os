#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include "../commons.h"

#define SOCKETPATH "/tmp/giladitaSocket"
#define LISTEN_BACKLOG 50
#define	MAX_CONNECTIONS 100

Connection * openConnection();
requestState writeRequest(Request * request, int fd);
int closeSocket(int fd) ;
int requestServer(Connection * connection, int action, int dataSize, void * data);
int listenConnection(Connection * connection);
Connection * createConnection(int fd);
Request * createRequest(int action, int fd, int dataSize, void * data);
#endif