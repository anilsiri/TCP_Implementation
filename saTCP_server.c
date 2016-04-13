//implementation of TCP over UDP/IP ----- server
#include<sys/types.h>
#include<sys/socket.h>
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
struct sockaddr_in client;
socklen_t server_len;
socklen_t client_len;


void threeWayHS(){
		int n;	
		char buffer[10];
		bzero(buffer, 255);
		char* token = (char*) malloc(10); 
		char syn_ack[10];
		strcpy(syn_ack, "SYN_ACK");
		char* seq = (char*) malloc(10);
		int seq_no;
		int ack_no;
		char* ack = (char*) malloc(10);
		
			client_len = sizeof(client);
		while(current_state == LISTEN){
			n = recvfrom(sock, buffer, strlen(buffer)+1, 0,(struct sockaddr*) &client, &client_len); //recieved SYN
			if(strcmp(buffer, "SYN") == 0){
				token = strtok(buffer," ");
				seq_no = atoi(token);
				seq_no = seq_no + 1;
				sprintf(seq, "%d", seq_no);
				sprintf(ack, "%d", ack_no);
				strcat(syn_ack, seq);
				strcat(syn_ack, ack);
			
			n = sendto(sock, syn_ack, strlen(syn_ack)+1, 0, (struct sockaddr*) &client, sizeof(client));
			current_state = SYN_RCVD;
		}
			else{
				//ignore it - cos it is not a SYN
			}
		}
		
		while(current_state == SYN_RCVD){
			bzero(buffer, strlen(buffer));
			n = recvfrom(sock, buffer, strlen(buffer)+1,0, (struct sockaddr*) &client, &client_len); //recieved SYN
			if(strcmp(buffer, "ACK") == 0){
				current_state = ESTABLISHED;
			}
		}
		
		//data_exchange();
		
	}
	
	
	
int main(int args, char* argv[]){
	int n;	
	server_len = sizeof(server);
	sock=socket(AF_INET,SOCK_DGRAM,0);
	
	
    if(sock < 0)
        {
                printf("Error opening socket\n");
                exit(1);
        }
	
	bzero((char*)&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORTNUM);
	
	n = bind(sock, (struct sockaddr*) &server, server_len);
	current_state = LISTEN; //start passive open
	threeWayHS();
}

