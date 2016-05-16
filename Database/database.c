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

// static const char * serverMsg[5] = {
//   "Estudiante agregado con exito",
//   "El estudiante ya existe",
//   "Estudiante modificado con exito",
//   "Estudiante eliminado con exito",
//   "Error en la base de datos"
// };

void processRequestDatabase (Request * request) {
  int state;
  printf("ACTION %d\n", request->action);
  switch (request->action) {

    case ADD_STUDENT:
      state = DbAddStudent(request->connection->data->name, request->connection->data->average);
      //int* responseFd;
      //char answerPipe[10] = "";
      //sprintf(answerPipe, "%d", request -> connection ->fd);
      printf("process request database\n");
      //responseFd = openNamedPipe(answerPipe);
      //write(responseFd[1], serverMsg[state], strlen(serverMsg[state]+1));
      //close(responseFd[1]);
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

