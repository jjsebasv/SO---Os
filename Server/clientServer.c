#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "../Api/namedPipe/namedPipe.h"
#include "../Api/commons.h"

int main (int argc, char const *argv[]){

	int listened = 0;
	Request * r;
	Connection* c;

	// //initialize request queue TODO REDO
	if( (c = openConnection(REQUEST_QUEUE)) == NOT_FOUND) {
		return ERROR_OPEN_REQUEST_QUEUE;
	}

	//sets up the fds to monitor
	monitorConnection(c, &set);

	//waits 2 seconds
	tv.tv_sec = 2;
  tv.tv_usec = 0;

	while(1) {
		listened = listenConnection(c);
		printf("listened: %d\n", listened);
		if( listened >=  0){
			r = getRequest(c);
			if( r != NOT_FOUND ){
				printf("Request procesada\n");
				processRequestServer(r);
			}
		} else{
			printf("Error\n");
		}
	}
	return 0;
}
