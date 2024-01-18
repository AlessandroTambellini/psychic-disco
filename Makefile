CC=gcc
CFLAGS=-Wall

all: server client

server: server.o program.o vm.o
	$(CC) $(CFLAGS) -o server server.o program.o vm.o

client: client.o program.o
	$(CC) $(CFLAGS) -o client client.o program.o

clean:
	rm -f server client *.o
