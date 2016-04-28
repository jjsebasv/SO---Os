SERVER=server.bin
SOURCES_NP=$(wildcard ../Api/namedPipe/*.c)
SOURCES_ST=$(wildcard ../Api/socket/*.c)

all:  $(SERVER)

$(SERVER):
	gcc $(SOURCES_ST) $(SOURCES_NP) -o $(SERVER) $(STATICLIBS)

clean:
	rm -rf  *.o *.bin 

.PHONY: rm -rf *.o *.bin
