#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>



static const int client_server_fd;

int main (void){

	fd_set set;  	//fds to monitor
	struct request r;
	struct timeval tv;		

	//initialize request queue
	if(!client_server_fd = openNamedPipe("RequestQueue")){

		return -1;
	}

	//sets up the fds to monitor
	FD_ZERO(&set);
	FD_SET(client_server_fd, &set);

	//waits 2 seconds
	tv.tv_sec = 2;
    tv.tv_usec = 0;



	while(1){

		//if something was written
		if(select(1, set, NULL, NULL, tv) > 0){ 

			r = getRequest(client_server_fd);

			if(r != NULL){

				processRequest(r);
			}	
		}else{

			printf("No requests to process\n");
		}      
	}
	return 0;
}



// TODO
struct request getRequest(int fd){
	return 0
}