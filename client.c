#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>


#include <unistd.h>

#define MAX 80
#define PORT_X 8080
#define PORT_0 8081 
#define SA struct sockaddr 

int option;
int connection=0;

int sockfd, connfd; 

struct sockaddr_in servaddr, cli; 

static unsigned char init_word[] = {0, 0, 0, 
                                    0, 0, 0, 
                                    0, 0, 0, 
                                    0};
                                    
void displayWinner();
void display();
void readFromTheKeyboard(int player);

void func() 
{ 
	read(sockfd,init_word,MAX);

		if(init_word[0]==1){
			while(1){			
				readFromTheKeyboard(1);
				display();
				write(sockfd,init_word,MAX);
				read(sockfd,init_word,MAX);
				displayWinner();					
			}
		}
		else
		{
			while(1){
				read(sockfd,init_word,MAX);
				displayWinner();
				readFromTheKeyboard(2);
				display();
				write(sockfd,init_word,MAX);
			}		
		} 
}

void connectToServer(int port){
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(port); 

	connection=1;	

	// connect the client socket to server socket 
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 			 
	} 
	else
		printf("connected to the server..\n"); 
}



void readFromTheKeyboard(int player){
	int check=0;

	display();

	do{	
		printf("Enter option!");
		scanf("%d", &option);
		if(init_word[option]==0){
			init_word[option]=player;
			check=1;
		}
	}while(check==0);
}

void displayWinner(){
	if(init_word[0]==3){
			printf("X WINS\n");
			exit(0);
	}
		
	if(init_word[0]==4){
		printf("0 WINS\n");
		exit(0);
	}
		
	if(init_word[0]==5){
		printf("DRAW\n");
		exit(0);		
	}	
}


void display(){
	int counter=1;
    for (int i=0; i<3;i++){
        printf("\n| ");
        for(int j=0;j<3;j++){
            if(init_word[counter]==1){
                printf("X |");
            }

            if(init_word[counter]==2){
                printf("O |");
            }

            if(init_word[counter]==0){
                printf("N |");
            }
            counter++;
        }
    }
}

int main() 
{ 
	
	// socket create and varification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("Socket can't be created!\n"); 
		exit(-1); 
	} 
	else{
		printf("Socket created successfully!\n"); 
	}

	connectToServer(PORT_X);
	if(connection==1){
		connectToServer(PORT_0);	
	}	
	func();
} 
