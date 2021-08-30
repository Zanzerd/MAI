all: client server
client: client.c ; gcc -Wall client.c -lzmq -o client -pthread
server: people.o server.c ; gcc -Wall server.c people.o -lzmq -o server
people.o : people.c people.h; gcc -c people.c
clean: ; rm -f *.o client server