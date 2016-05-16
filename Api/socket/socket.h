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

Connection * openConnection();
requestState writeRequest(Request * request, int fd);
int closeSocket(int fd) ;
int requestServer(Connection * connection, int action, size_t dataSize, void * data);
int listenConnection(Connection * connection);
Connection * createConnection(int fd);
Request * createRequest(int action, int fd, size_t dataSize, void * data);
#endif