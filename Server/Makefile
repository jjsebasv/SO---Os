SERVER=server.bin
SOURCES_API=$(wildcard ../Api/*.c)
SOURCES_NP=$(wildcard ../Api/namedPipe/*.c)
SOURCES_ST=$(wildcard ../Api/socket/*.c)
SOURCES_SERVER=$(wildcard ./*.c)

allST: $(SERVERST)

allNP: $(SOURCES_NP) $(SOURCES_SERVER) $(SOURCES_API)
	gcc $(SOURCES_NP) $(SOURCES_SERVER) $(SOURCES_API) -o $(SERVER)

$(SERVERST): $(SOURCES_ST) $(SOURCES_SERVER) $(SOURCES_API)
	gcc $(SOURCES_ST) $(SOURCES_SERVER) $(SOURCES_API) -o $(SERVER)

clean:
	rm -rf  *.o *.bin 

.PHONY: clean
