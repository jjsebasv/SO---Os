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
  switch (request->action) {

    case ADD_STUDENT:
      state = DbAddStudent(request->connection->data->name, request->connection->data->average);
      printf("Procensado request DB ...\n");
      writeResponse(request, state);
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

