#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include<netinet/in.h> 
#include<unistd.h> 
#include<stdlib.h> 

// Client UDP  
int main() 
{    
    	char message[100] = {}; 
    	int sockfd, n; 
    	struct sockaddr_in serveraddr; 
      
    	// clear servaddr 
    	bzero(&serveraddr, sizeof(serveraddr));

    	// Setup server information 
    	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    	serveraddr.sin_port = htons(50000); 
    	serveraddr.sin_family = AF_INET; 
      
    	// create socket using SOCK_DGRAM 
    	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
      
    	// connect to server 
    	if(connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) 
    	{ 
        	printf("\n Error: Connect failed \n"); 
        	exit(0); 
    	} 
 
    	while(1){ 
    	printf("Client Message: ");
    	scanf("%[^\n]%*c", message);

	// Send data to server but because we are using UDP we do not need to specify server address, we also check in the if statement if the message was sent correctly
    	if (sendto(sockfd, message, 1000, 0, (struct sockaddr*)NULL, sizeof(serveraddr)) < 0){
		printf("[+] Send failed");
		exit(1);
    	} 

    	char server_reply[100] = {};      
   	// waiting for response 
    	if (recvfrom(sockfd, server_reply, sizeof(server_reply), 0, (struct sockaddr*)NULL, NULL) < 0){
		printf("[+] Recv failed");
		exit(1);
    	} 
	
    	printf("%s\n", server_reply); 

    	}
  
    	// close the descriptor 
    	close(sockfd); 
} 
