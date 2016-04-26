#TODO CHOOSE BETWEEN NAMEDPIPE OR SOCKET IMPLEMENTATION
CLIENT=client.bin
SOURCES_NP=$(wildcard ../Api/namedPipe/*.c)
SOURCES_ST=$(wildcard ../Api/socket/*.c)

all:  $(CLIENT)

$(CLIENT):
	gcc $(SOURCES_ST) $(SOURCES_NP) -o $(CLIENT) $(STATICLIBS)

clean:
	rm -rf  *.o *.bin 

.PHONY: all clean