#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

// Type - whether is a file / IP / PIPE / Named Pipe
// Direction - where to look for the data
// Action - whethet to read or write

typedef struct Request {
  int action;
  size_t dataSize;
  void* data;
  size_t directionSize;
  void* direction;
} Request;

// To consider: Should the request have data to write?
// If the request demands to write in a file for the user to read, then
// there should be something in the data.
// If the request demands to read from the file that the user wrote, then
// data could be null and filled with the information gotten.

//direction: file's name / IP address / PIPE's fd / Named Pipe's name
typedef struct Response {
  int directionSize;
  int responseSize;
  char * direction;
  char * response;
} Response;


//the client should use this function to start a request
//request is initialized and sent to the server
int requestServer(Response * response, int action, int type, size_t dataSize, void * data);

// Write a request in the request queue
int writeRequest (Request * request);

// Get the first request in the request queue
int getRequest(Request * request);

// Process a request
int processRequest(Request * r);

// Gets the response from the server
int getReponse(Response * response)

requestState readRequest(Request request);

requestState writeRequest(Request request);

