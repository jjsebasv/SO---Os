CLIENT=client.bin
SOURCES_API=$(wildcard ../Api/*.c)
SOURCES_NP=$(wildcard ../Api/namedPipe/*.c)
SOURCES_ST=$(wildcard ../Api/socket/*.c)
SOURCES_CLIENT=$(wildcard ./*.c)

allST: $(CLIENTST)

allNP: clientNP

clientNP: $(SOURCES_NP) $(SOURCES_CLIENT) $(SOURCES_API)
	gcc $(SOURCES_NP) $(SOURCES_CLIENT) $(SOURCES_API) -o $(CLIENT)

$(CLIENTST): $(SOURCES_ST) $(SOURCES_CLIENT) $(SOURCES_API)
	gcc $(SOURCES_ST) $(SOURCES_CLIENT) $(SOURCES_API) -o $(CLIENT)

clean:
	rm -rf  *.o *.bin 

.PHONY: allNP allST clean clientNP
