CLIENT=client.bin
SOURCES_API=$(wildcard ../Api/*.c)
SOURCES_NP=$(wildcard ../Api/namedPipe/*.c)
SOURCES_ST=$(wildcard ../Api/socket/*.c)
SOURCES_CLIENT=$(wildcard ./*.c)

allST:  $(CLIENTST)

allNP: $(CLIENTNP)

$(CLIENTNP):
	gcc $(SOURCES_NP) $(SOURCES_CLIENT) $(SOURCES_API) -o $(CLIENT)

$(CLIENTST):
	gcc $(SOURCES_ST) $(SOURCES_CLIENT) $(SOURCES_API) -o $(CLIENT)

clean:
	rm -rf  *.o *.bin 

.PHONY: rm -rf *.o *.bin
