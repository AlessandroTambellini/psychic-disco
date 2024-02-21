CC=gcc
CFLAGS=-Wall
INSTALL_PATH=/usr/bin

all: server client

server: server.o program.o vm.o el.o
	$(CC) $(CFLAGS) -o server server.o program.o vm.o el.o

client: client.o program.o
	$(CC) $(CFLAGS) -o client client.o program.o

test: client
	sh test.sh

clean:
	rm -f server client *.o

install: server
	echo Installing executable to ${INSTALL_PATH}
	mv server ${INSTALL_PATH}/server

uninstall:
	echo Removing executable from ${INSTALL_PATH}
	rm ${INSTALL_PATH}/server
