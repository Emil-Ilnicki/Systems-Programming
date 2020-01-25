#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <strings.h> 
#include <ctype.h>
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 

int add(int a, int b);
int sub(int a, int b);
int mul(int a, int b);
int divi(int a, int b);

// Server UDP  
int main() 
{    
    	int listenfd, len; 
	struct sockaddr_in serveraddr, clientaddr; 
	bzero(&serveraddr, sizeof(serveraddr)); 
  
 	   // Create a UDP Socket 
    	listenfd = socket(AF_INET, SOCK_DGRAM, 0);         
    	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    	serveraddr.sin_port = htons(50000); 
    	serveraddr.sin_family = AF_INET;  
   
    	// bind server address to socket descriptor 
    	bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)); 
       
    	//receive the datagram 
    	len = sizeof(clientaddr);

    	char *clientpacket[3];
    	char errormessage[] = "This is not a supported opperand";
	char *operand[100];
    	int num1;
    	int num2;
    	int result;
 
    	while(1){

    	char buffer[100] = {};
    	char serverpacket[100] = {};
	int n = recvfrom(listenfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientaddr,&len); 
    	buffer[n] = '\0'; 
	
    	char *token = strtok(buffer, " ");
    	int i = 0;
 
	// While loop that tokenizes buffer for easier use
    	while(token != NULL){
		clientpacket[i] = token;
		token = strtok(NULL, " ");
		i++;
    	}

	// While loop that changes operand characters to lower case
	i = 0;
	while(i < 3){
		clientpacket[0][i] = tolower(clientpacket[0][i]);
		i++;
	}
	

	// checks if first index of clientpacket is equal to add
    	if(strcmp(clientpacket[0], "add") == 0){
		num1 = atoi(clientpacket[1]); // convert second index from string to int
		num2 = atoi(clientpacket[2]); // convert third index from string to int
		result = add(num1,num2); // call add method and pass in num1 and num2
		sprintf(serverpacket, "1: (%d) + (%d) = %d", num1, num2, result); // convert int to string and store into serverpacket buffer
        	sendto(listenfd, serverpacket, 1000, 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr)); // send result back to client	
    	}
	// checks if first index of clientpacket is equal to sub
	else if(strcmp(clientpacket[0], "sub") == 0){
		num1 = atoi(clientpacket[1]);
                num2 = atoi(clientpacket[2]);
                result = sub(num1,num2);
                sprintf(serverpacket, "1: (%d) - (%d) = %d", num1, num2, result);
                sendto(listenfd, serverpacket, 1000, 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
	} 
	// checks if first index of clientpacket is equal to mul
	else if(strcmp(clientpacket[0], "mul") == 0){
		num1 = atoi(clientpacket[1]);
                num2 = atoi(clientpacket[2]);
                result = mul(num1,num2);
                sprintf(serverpacket, "1: (%d) * (%d) = %d", num1, num2, result);
                sendto(listenfd, serverpacket, 1000, 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
	}
	// checks if first index of clientpacket is equal to div
	else if(strcmp(clientpacket[0], "div") == 0){
		num1 = atoi(clientpacket[1]);
                num2 = atoi(clientpacket[2]);
		// if num2 equals 0 we send back the client a cannot divide by 0 message
		if (num2 == 0){ 
			char serverpacket[] = "0: Cannot divide by 0";
			sendto(listenfd, serverpacket, 1000, 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
		} 
		// else the numbers passed in can be divided
		else { 
                result = divi(num1,num2);
                sprintf(serverpacket, "1: (%d) / (%d) = %d", num1, num2, result);
                sendto(listenfd, serverpacket, 1000, 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
		}
	}
    	else {
		sendto(listenfd, errormessage, 1000, 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));     
    	} 
    }
} 

int add(int a, int b){
    return (a+b);
}
int sub(int a, int b){
    return (a-b);
}
int mul(int a, int b){
    return (a*b);
}
int divi(int a, int b){
    return (a/b);
}
