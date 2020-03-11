#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 


#define PORT_X 8080
#define PORT_0 8081 
#define SA struct sockaddr 

int connection=0;
static unsigned char init_word[] = {0, 0, 0, 
                                    0, 0, 0, 
                                    0, 0, 0, 
                                    0};
void func(int sockfd) 
{ 
	char buff[MAX]; 
	int n; 
	for (;;) { 
		bzero(buff, sizeof(buff)); 
		printf("Enter the string : "); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n') 
			; 
		write(sockfd, buff, sizeof(buff)); 
		bzero(buff, sizeof(buff)); 
		read(sockfd, buff, sizeof(buff)); 
		printf("From Server : %s", buff); 
		if ((strncmp(buff, "exit", 4)) == 0) { 
			printf("Client Exit...\n"); 
			break; 
		} 
	} 
} 

int connect(int port){
	bzero(&servaddr, sizeof(servaddr)); 

		// assign IP, PORT 
		servaddr.sin_family = AF_INET; 
		//////////////////////////////////////////////////
		//////////////////////////////////////////////////
		//IP UL TAU//////////////////////////////////////////
		////////////////////////////////////////////
		/////////////////////////////
		servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
		servaddr.sin_port = htons(PORT); 

		// connect the client socket to server socket 
		if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
			printf("connection with the server failed...\n"); 
			exit(0); 
		} 
		else
			printf("connected to the server..\n"); 

		// function for chat 
		func(sockfd); 

		// close the socket 
		close(sockfd);
		connection=1;
}

void readFromTheKeyboard(int player){
	printf("Enter option!");
	scanf("%d", option);
	if(init_word[option-1]==0){
		init_word[option-1]=player;
	}
}

void displayWinner(){
	if(init_word[9]==2){
		printf("X wins");
	}
	if(init_word[9]==3){
		printf("0 wins");
	}
}

int main() 
{ 
	int option;
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 

	// socket create and varification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("Socket can't be created!\n"); 
		exit(-1); 
	} 
	else
		printf("Socket created successfully!\n"); 
	if(connection==0){
		connect(PORT_X);
		while(1){
			readFromTheKeyboard(1);
			//METODA CU SERVERUL
			displayWinner();
		}
	}
	else{
		connect(PORT_0);
		connection=0;
		while(1){
			readFromTheKeyboard(2);
			//METODA CU SERVERUL
			displayWinner();
		}
	}
	
} 
