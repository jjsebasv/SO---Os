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


#define REQUEST_QUEUE "giladita"

#define BLOCK 1024 

int openNamedPipe(char * something);

void writeNamedPipe(int fd, void * data, int size);

int readNamedPipe (int fd, char * buffer);

int closeNamedPipe(int fd, char * something);

#endif