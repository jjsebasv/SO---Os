#ifndef _NAMED_PIPE_H_
#define _NAMED_PIPE_H_

// From http://stackoverflow.com/questions/2784500/how-to-send-a-simple-string-between-two-programs-using-pipes
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

//the client should use this function to start a request
//request is initialized and sent to the server
Request * createRequest(int action, int fd, int dataSize, void * data);

Connection * createConnection(int fd);

/**
 *  Returns two fds for the namedPipeName
 *
 *  @param  namedPipeName name of the pipe
 */
int * openNamedPipe(char * something);

void writeNamedPipe(int fd, void * data, int size);

int readNamedPipe (int fd, void * buffer);

int closeNamedPipe(int fd, char * something);

Request * getRequest(Connection * connection);

#endif