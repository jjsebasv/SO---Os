typedef enum connections {PIPE = 0, SOCKET, NAMED_PIPE, IP} connectionType;
enum requestStates {REQUEST_OK = 200, REQUEST_INVALID_TYPE, FAILED_ON_CREATE_REQUEST} requestState;

// Type - whether is a file / IP / PIPE / Named Pipe
// Direction - where to look for the data
// Action - whethet to read or write

struct Request {
  int action;
  int type;
  size_t dataSize;
  void* data;
  size_t directionSize;
  void* direction;
};

// To consider: Should the request have data to write?
// If the request demands to write in a file for the user to read, then
// there should be something in the data.
// If the request demands to read from the file that the user wrote, then
// data could be null and filled with the information gotten.

/* [ Alternative ]

struct Request {
  int action;
  void* data;
  void* direction;
  int type;
}
*/

//the client should use this function to start a request
//request is initialized and sent to the server
//returns the fd where the response will be read TODO Response structure
struct Response requestServer(int action, int type, size_t dataSize, void* data);

// Write a request in the request queue
int writeRequest (Request * request);

// Get the first request in the request queue
struct Request getRequest(int fd);

// Process a request
void processRequest(struct Request r);

requestState readRequest(Request r);

requestState writeRequest(Request r);

