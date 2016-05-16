#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "../Api/commons.h"
#include "../Database/databaseapi.h"

void processRequestServer (Request * request) {
  int pid = fork();
  if (pid == 0) {
    // Datos - vamo a procesar la request
  	processRequestDatabase(request);
    kill(getpid(), SIGKILL);
  } else if (pid > 0) {
    // Server - vamo a agarrar otra request
  } else {
    // fork error
  }
}

int main (int argc, char const *argv[]){

	int listened = 0;
	Request * r;
	Connection* c;

	// //initialize request queue TODO REDO
	if( (c = openConnection()) == NOT_FOUND) {
		return ERROR_OPEN_REQUEST_QUEUE;
	}
	printf("el fd es %d\n", c -> fd);

	while(1) {
		printf("while 1\n");
		listened = listenConnection(c);
		printf("listened: %d\n", listened);
		if( listened >=  0){
			r = getRequest(c, listened);
			if( r != NOT_FOUND ){
				printf("vamo a procesar\n");
				processRequestServer(r);
			}
		} else{
			printf("Error\n");
		}
	}
	return 0;
}