all: 
	g++ -o server saTCP_server.c
	g++ -o client saTCP_client.c
	
clean: 
	rm server
	rm client
	