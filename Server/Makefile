SERVER=server.bin
SOURCES_API=$(wildcard ../Api/*.c)
SOURCES_NP=$(wildcard ../Api/namedPipe/*.c)
SOURCES_ST=$(wildcard ../Api/socket/*.c)
SOURCES_SERVER=$(wildcard ./*.c)

allST: $(SERVERST)

allNP: $(SERVERNP)

$(SERVERNP):
	gcc $(SOURCES_NP) $(SOURCES_SERVER) $(SOURCES_API) -o $(SERVER)

$(SERVERST):
	gcc $(SOURCES_ST) $(SOURCES_SERVER) $(SOURCES_API) -o $(SERVER)

clean:
	rm -rf  *.o *.bin 

.PHONY: clean
