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
#define NAMED_PIPE_QUEUE "name"

// To consider: Should the request have data to write?
// If the request demands to write in a file for the user to read, then
// there should be something in the data.
// If the request demands to read from the file that the user wrote, then
// data could be null and filled with the information gotten.

//direction: file's name/ IP address / PIPE's fd
typedef struct Connection{
  int responseSize;
  int connection;
  char * response;
}Connection;


int openNamedPipe(char * something);

void writeNamedPipe(int fd, void * data, int size);

void readNamedPipe (int fd);

int closeNamedPipe(int fd, char * something);
