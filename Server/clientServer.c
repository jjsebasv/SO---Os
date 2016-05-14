#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "../Api/commons.h"

int main (int argc, char const *argv[]){

	int listened = 0;
	Request * r;
	Connection* c;

	// //initialize request queue TODO REDO
	if( (c = openConnection()) == NOT_FOUND) {
		return ERROR_OPEN_REQUEST_QUEUE;
	}

	while(1) {
		listened = listenConnectin(c);
		printf("listened: %d\n", listened);
		if( listened >=  0){
			r = getRequest(c);
			if( r != NOT_FOUND ){
				// TODO processRequest(r);
			}
		} else( listened < 0 ){
			printf("Error\n");
	}
	return 0;
}
