#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

// TCP client

int main() {
	
	int sockfd=socket(PF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr; // creating class called addr

	// preparing socket 
	addr.sin_family=PF_INET; // address family
	addr.sin_port=htons(12345); // port number (big endian)
	inet_aton("127.0.0.1",&addr.sin_addr);

	// connection
	connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
	printf("[+] Connected\n");

	// communication
	printf("[+] Messages want to send to server\n");
	char message[100]={};
	char server_reply[100]={};

	while(1){
			
		printf("Client Message: ");
		// reads in user input from terminal but it does not cut  on spaces so it is read as add 1 1 and not add, 1, 1
		scanf("%[^\n]%*c", message);
			
		// sends client message to the server, if it fails we exit the program
		if (send (sockfd, message, strlen(message)+1, 0) < 0){
			puts("[+] send failed");
			exit(1);
		}

	        char server_reply[100] = {};
		// read message from server 
		if (recv(sockfd, server_reply, 2000, 0) < 0){
			puts("[+] recv failed");
			exit(1);
		}
		// print message from server	
		printf("%s\n", server_reply);
	}
	close(sockfd);
}
