all: client server
CC =  gcc
CFLAGS = -g

client:
	gcc -g -o client client.c

server:
	gcc -g -o server server.c

clean:
	rm -f server client fifo_temp
