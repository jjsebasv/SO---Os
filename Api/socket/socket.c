#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include "socket.h"
#include "../commons.h"

#define SOCKETPATH "/tmp/giladitaSocket"

Connection * openConnection(){
	Connection * connection;
	struct sockaddr_un myAddr;
	int socketfd;

	socketfd = socket(AF_UNIX, SOCK_STREAM /*| SOCK_NONBLOCK*/, 0);
	if(socketfd == -1){
		return NULL;
	}
	memset(&my_addr, 0, sizeof(struct sockaddr_un));
	my_addr.sun_family = AF_UNIX;
	strncpy(my_addr.sun_path, SOCKETPATH, sizeof(my_addr.sun_path) - 1);
}