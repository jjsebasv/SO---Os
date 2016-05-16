#ifndef _NAMED_PIPE_H_
#define _NAMED_PIPE_H_

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include "../commons.h"

#define NOT_FOUND_ERR 0
#define REQUEST_QUEUE "giladita"
#define BLOCK 1024 


Request * createRequest(int action, int fd, int dataSize, void * data);

Connection * createConnection(int fd);

void processRequestServer (Request * request);

int * openNamedPipe(char * something);

void writeNamedPipe(int fd, void * data, int size);

int readNamedPipe (int fd, void * buffer);

int closeNamedPipe(int fd, char * something);

Request * getRequest(Connection * connection, int listened);

int writeResponse (Request * request, int state);

#endif