CC=gcc
CFLAGS=-Wall

all: server client

server: server.o program.o vm.o msg.o
	$(CC) $(CFLAGS) -o server server.o program.o vm.o msg.o

client: client.o program.o msg.o
	$(CC) $(CFLAGS) -o client client.o program.o msg.o

clean:
	rm -f server client *.o
