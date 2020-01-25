#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
	
int add(int a, int b);
int sub(int a, int b);
int mul(int a, int b);
int divi(int a, int b);

// TCP server

int main(){

	// Create a socket
	int sockfd=socket(PF_INET,SOCK_STREAM,0);
	// prepare address
	struct sockaddr_in addr; 
	addr.sin_family=PF_INET;
	// Port numbers such as 8008 are reserved by larger companies and thus we have to create our own port number.
	addr.sin_port= htons(12345);
	//addr_sin_addr =("127.0.0.1")
	inet_aton("127.0.0.1",&addr.sin_addr);
	//3rd step is to bind socket and address
	int res=bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
	if(res==-1){
		perror("[+] Bind failed");
		exit(-1);
	}
	printf("[+] Bind successful\n");
	//4th step is to listen to the client
	if(listen(sockfd,100)==-1){
		perror("listen failed");
		exit(1);
	}
	printf("[+] Starting to listen \n");
	// wait for client to connect, client addr stored in fromaddr
	struct sockaddr_in fromaddr;
	socklen_t len=sizeof(fromaddr);

	int rd;
	char *clientpacket[3];
	char errormessage[] = "This is not a supported opperand";
	int num1;
	int num2;
	int result;

        int fd=accept(sockfd,(struct sockaddr*)&fromaddr,&len);
	if (fd == -1){
		perror("Accept failed");
	}
	else {
		printf("[+] Client has successfully connected to the server\n");
	}

			
	while (1){
		
		
		char buf[90] = {};
		char serverpacket[100];
		// read client information 
		rd=read(fd,buf,sizeof(buf));
		buf[rd]='\0';
	
		// tokenize client string using a delimiter " "
		char *token = strtok(buf, " ");
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
		
		// check if the first index of the tokenized string is equal to "add"
		if(strcmp(clientpacket[0],"add") == 0){
			num1 = atoi(clientpacket[1]); // convert string to int
			num2 = atoi(clientpacket[2]); // convert string to int
			result = add(num1,num2); // call add method and pass in two variables num1 and num2
			sprintf(serverpacket, "1: (%d) + (%d) = %d", num1, num2, result); // convert result into string type and store it in serverpacket buffer
			write(fd,serverpacket,strlen(serverpacket)); // write the result back to the client 
			
		// check if the first index of the tokenized string is equal to "sub"
		} else if(strcmp(clientpacket[0],"sub") == 0){
			num1 = atoi(clientpacket[1]);
			num2 = atoi(clientpacket[2]);
			result = sub(num1,num2); // call sub method
			sprintf(serverpacket, "1: (%d) - (%d) = %d", num1, num2, result);
			write(fd,serverpacket,strlen(serverpacket));

		// check if the first index of the tokenized string is equal to "mul"
		} else if (strcmp(clientpacket[0],"mul") == 0){
			num1 = atoi(clientpacket[1]);
			num2 = atoi(clientpacket[2]);
			result = mul(num1,num2); // call mul method
			sprintf(serverpacket, "1: (%d) * (%d) = %d", num1, num2, result);
			write(fd,serverpacket,strlen(serverpacket));

		// check if the first index of the tokenized string is equal to "div"
		} else if (strcmp(clientpacket[0],"div") == 0){
			num1 = atoi(clientpacket[1]);
			num2 = atoi(clientpacket[2]);
			// if num2 is equal to 0 we know we will encounter a divide by 0 error
			if (num2 == 0){
				char serverpacket[] = "0: Cannot divide by 0"; 
				write(fd,serverpacket,strlen(serverpacket)); // write back to client that divison by 0 is an error
			} else {
			result = divi(num1,num2); // else call div method 
			sprintf(serverpacket, "1: (%d) / (%d) = %d", num1, num2, result);
			write(fd,serverpacket,strlen(serverpacket));
			}

		} else { // if the client did not enter in a valid operand we send them an error message
			write(fd,errormessage,strlen(errormessage)); 
		}
		
	}	
	close(sockfd); // close socket
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




