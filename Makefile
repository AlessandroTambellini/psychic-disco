CC=gcc
CFLAGS=-Wall
INSTALL_PATH=/usr/bin
SERVER_NAME=pd-server

all: server client repl

server: server.o program.o vm.o el.o
	$(CC) $(CFLAGS) -o server server.o program.o vm.o el.o

client: client.o program.o
	$(CC) $(CFLAGS) -o client client.o program.o

repl: repl.o program.o
	$(CC) $(CFLAGS) -o repl repl.o program.o

test: server client
	sh test.sh

clean:
	rm -f server client repl *.o

install: server
	echo Installing executable to ${INSTALL_PATH}
	mv server ${INSTALL_PATH}/${PD_SERVER}

uninstall:
	echo Removing executable from ${INSTALL_PATH}
	rm ${INSTALL_PATH}/${PD_SERVER}
