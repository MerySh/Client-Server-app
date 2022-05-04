all: server.o client.o
	gcc CLI.c server.c -o server
	gcc CLI.c client.c -o client

clean:
	rm *.o server client 
