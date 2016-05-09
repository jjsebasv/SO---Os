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
	int client_server_fd;
	Request * r;
	struct timeval tv;
	Connection *c;

	// //initialize request queue TODO REDO
	if( c = openConnection(c)) {
		return ERROR_OPEN_REQUEST_QUEUE;
	}

	//sets up the fds to monitor
	FD_ZERO(&set);
	FD_SET(client_server_fd, &set);

	//waits 2 seconds
	tv.tv_sec = 2;
  	tv.tv_usec = 0;

	while(1){
		// if something was written
		if( select(1, &set, NULL, NULL, &tv) > 0){
			// TO DO
			// r = getRequest(client_server_fd);
			if( r != NULL ){
				processRequest(r);
			}
		} else {
			printf("No requests to process\n");
		}
	}
	return 0;
}