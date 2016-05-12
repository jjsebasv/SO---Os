#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include "../Api/commons.h"
#include "../Api/request.h"


int main (int argc, char const *argv[]){

	fd_set set;  	//fds to monitor
	Request * r;
	struct timeval tv;
	Connection *c;
	fd_set jorge;

	// //initialize request queue TODO REDO

	if( (c = openConnection()) == NOT_FOUND) {
		return ERROR_OPEN_REQUEST_QUEUE;
	}

	printf("fd connection %d\n", c->np->fd);
	//sets up the fds to monitor
	monitorConnection(c, &set);
 	FD_ZERO(&jorge);
	//waits 2 seconds
	tv.tv_sec = 2;
  	tv.tv_usec = 0;

	while(1){

		int fdCount = select(c -> np -> fd + 1, &set, NULL, NULL, &tv);
			tv.tv_sec = 2;
			monitorConnection(c, &set);
		printf("fdCount: %d\n", fdCount);
		if( fdCount > 0){

			r = getRequest(c);
			if( r != NOT_FOUND ){
				printf("hodi\n");
				// TODO processRequest(r);
			}
		}if(fdCount == 0) {
			printf("No requests to process\n");
		}else{
			printf("error\n");
		}
	}
	return 0;
}