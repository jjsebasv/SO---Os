// who - where the connection should be opened
// returns a positive int with the 'connection id'
// returns -1 if not listening
int open(void* who);

// data - a pointer to the information to be sent
// size - how long the information to be read is
// who - 'connection id' from where to send
// returns 0 if data was sent correctly
int send(void* data, int size, int who);

// pointer - where the information would be saved
// size - block size / size to save
// who - where the information is
// returns the amount of bytes saved or -1 if failed
int recieve(void* pointer, int size, int who);

// who - which connection should be closed
// returns 0 if closed correctly
int close(int who);
