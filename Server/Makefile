SERVER=server.bin
SOURCES_API=$(wildcard ../Api/*.c)
SOURCES_NP=$(wildcard ../Api/namedPipe/*.c)
SOURCES_ST=$(wildcard ../Api/socket/*.c)
SOURCES_SERVER=$(wildcard ./*.c)
SOURCES_DB=$(wildcard ../Database/*.c)

allST: $(SOURCES_ST) $(SOURCES_SERVER) $(SOURCES_API) $(SOURCES_DB) 
	gcc $(SOURCES_ST) $(SOURCES_SERVER) $(SOURCES_API) $(SOURCES_DB) -o $(SERVER) -lsqlite3 -Wall

allNP: $(SOURCES_NP) $(SOURCES_SERVER) $(SOURCES_API) $(SOURCES_DB)
	gcc $(SOURCES_NP) $(SOURCES_SERVER) $(SOURCES_API) $(SOURCES_DB) -o $(SERVER) -lsqlite3 -Wall

clean:
	rm -rf  *.o *.bin 

.PHONY: clean
