#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netdb.h>

#define PORTNUM 5678

enum state{CLOSED, LISTEN, SYN_RCVD, SYN_SENT, ESTABLISHED, CLOSING};
enum state current_state; //to indicate the current state 

int sock;
struct sockaddr_in server; 
socklen_t length;
struct sockaddr_in client;
socklen_t client_len;

void threeWayHandshake(){
	current_state = SYN_SENT;
}


void connection(char* ip){
	char* ip_addr = (char*) malloc(30);
	char* token = (char*) malloc(5);
	int port;
	ip_addr = strtok(ip, ":");
	token = strtok(NULL, "\0");
	port = atoi(token);
	
	sock = socket(AF_INET, SOCK_DGRAM,0);
	if(sock < 0)
        {
                printf("Error opening socket\n");
                exit(1);
        }
	
	bzero((char*)&server, sizeof(server));
	
	struct hostent* hp;
	hp = gethostbyname(ip_addr);
	if(hp==0)
	{

		printf("Unknown host\n");
	}
	
	bcopy((char *)hp->h_addr,(char *)&server.sin_addr,hp->h_length);
	server.sin_family = AF_INET;
 	server.sin_port = htons(port);

	threeWayHandshake();
}

int main(int argc, char* argv[]){
		//for connection
		if(argc == 2){
				if(strcmp(argv[1],"connect") == 0){
					connection(argv[2]);
				}
		}
}
