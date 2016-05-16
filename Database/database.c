#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "../Api/commons.h"
#include "../Api/namedPipe/namedPipe.h"
#include "databaseapi.h"

void processRequestDatabase (Request * request) {
  int state;
  printf("Procensado request DB ...\n");
  switch (request->action) {

    case ADD_STUDENT:
      state = DbAddStudent(request->connection->data->name, request->connection->data->average);
      writeResponse(request, state);
      break;

    case DELETE_STUDENT:
      state = DbDeleteStudent(request->connection->data->name);
      writeResponse(request, state);
      break;

    case READ_STUDENTS:
      DbReadStudents();
      break;

    case DROP_TABLE:
      DbDropTable();
      break;

    case CREATE_TABLE:
      state = DbCreateTable();
      writeResponse(request, state);
      break;

    default:
      printf("Error request action\n");
  }
}

