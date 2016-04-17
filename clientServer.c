#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>


static const int client_server_fd;

int main (void){

	fd_set set;  	//fds to monitor
	struct Request * r;
	struct timeval tv;

	//initialize request queue
	if( !client_server_fd = openNamedPipe("RequestQueue") ) {

		return -1;
	}

	//sets up the fds to monitor
	FD_ZERO(&set);
	FD_SET(client_server_fd, &set);

	//waits 2 seconds
	tv.tv_sec = 2;
  tv.tv_usec = 0;

	while(1){
		// if something was written
		if( select(1, set, NULL, NULL, tv) > 0){
			r = getRequest(client_server_fd);
			if( r != NULL ){
				processRequest(r);
			}
		} else {
			printf("No requests to process\n");
		}
	}
	return 0;
}

struct Request getRequest(int fd) {
	int aux_err;
	struct Request * rta;

	aux_err = read( fd, rta, sizeof(struct Request) );
	if ( aux_err )
		return rta;
	return NOT_FOUND_ERR; // return NULL
}

// action = 0 read
// action = 1 write
void processRequest(struct Request r) {
	switch (r -> action) {
		case 0:
			return readRequest(r);
			break;
		case 1:
			return writeRequest(r);
			break;
	}
	return;
}



// TODO
// #define NOT_FOUND_ERR
// int processRequest(struct Request r) {};
// readRequest(r)
// writeRequest(r)
