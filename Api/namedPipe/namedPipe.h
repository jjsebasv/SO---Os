// From http://stackoverflow.com/questions/2784500/how-to-send-a-simple-string-between-two-programs-using-pipes
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>


#define BLOCK 1024 
#define NAMED_PIPE_QUEUE "name"

int openNamedPipe(char * something);

void writeNamedPipe(int fd, void * data, int size);

void readNamedPipe (int fd);

int closeNamedPipe(int fd, char * something);
