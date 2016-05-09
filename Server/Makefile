SERVER=server.bin
SOURCES_NP=$(wildcard ../Api/namedPipe/*.c)
SOURCES_ST=$(wildcard ../Api/socket/*.c)
SOURCES_SERVER=$(wildcard ./*.c)

all:  $(SERVER)

$(SERVER):
	gcc $(SOURCES_ST) $(SOURCES_NP) $(SOURCES_SERVER) -o $(SERVER)

clean:
	rm -rf  *.o *.bin 

.PHONY: rm -rf *.o *.bin
