#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "../Api/namedPipe/namedPipe.h"
#include "../Api/commons.h"

void processRequestServer (Request * request) {
  int pid = fork();
  if (pid == 0) {
    // Server - vamo a procesar la request
    processRequestDatabase(request);
    //kill(getpid(), SIGKILL);
  } else if (pid > 0) {
    // Server - vamo a agarrar otra request
    // Server - master
  } else {
    // fork error
  }
}

void processRequestDatabase (Request * request) {
  int state;
  switch (request->action) {

    case ADD_STUDENT:
      state = DbAddStudent(request->connection->data->name, request->connection->data->average);
      printf("Vamo a escribir\n");
      printf("STATE %d\n", state);
      printf("MENSAJE %s\n", serverMsg[state]);
      printf("CONNECTION %d\n", request->connection->fd);
      int numero = 7;
      int escrito = write(request->connection->fd, &numero, sizeof(int));
      printf("%d\n", escrito);
      break;

    case UPDATE_STUDENT:
      //DbUpdateStudent();
      break;

    case DELETE_STUDENT:
      DbDeleteStudent(request->connection->data->name);
      break;

    case READ_STUDENTS:
      DbReadStudents();
      break;

    case DROP_TABLE:
      DbDropTable();
      break;

    case CREATE_TABLE:
      DbCreateTable();
      break;

    default:
      printf("Error request action\n");
  }
}

int main (int argc, char const *argv[]){

	fd_set set;  	//fds to monitor
	Request * r;
	struct timeval tv;
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

		time_t timer;
    char buffer[26];
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y:%m:%d %H:%M:%S", tm_info);
    printf("%s ", buffer);

		int fdCount = select(c -> fd + 1, &set, NULL, NULL, &tv);
		tv.tv_sec = 2;
		monitorConnection(c, &set);
		if( fdCount > 0){
			r = getRequest(c);
			if( r != NOT_FOUND ){
				printf("Request procesada\n");
				processRequestServer(r);
			}
		} else if ( fdCount < 0 ){
			printf("Error\n");
		} else {
			printf("No requests to process\n");
		}
	}

	return 0;
}
