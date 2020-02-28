/*
 * Proiec PAD - Joc X si 0
 * Echipa 
 *  Tutelea Dan
 *  Pescaru Alexandru   
 *  Feier Denis
 *  Gergelyfi Levente
 *  @copyright, 2020
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//pentru comunicare
#include <unistd.h>

//pentru net
#include <netdb.h>
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>

// => porturile sunt 8080 si 8081
#define PORT(deltaP) (8080+deltaP) 
#define CLIENT_NO_1 0
#define CLIENT_NO_2 1

struct clinet_descriptor{
    int iofd; //file descriptor to be used with read and write
    int socketfd; //socket file clinet_descriptor
    int len; //client data
    struct sockaddr_in serveraddr; //server data
    struct sockaddr_in clientaddr; //client data
};

void init_conversation(struct clinet_descriptor *client, const char client_no){
    static unsigned char init_word[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    /*
     * Creare socket
     * */
    client->socketfd = socket(AF_INET, SOCK_STREAM, 0); 
    if(client->socketfd<0){
        printf("Socket for client %d creation failed...\n", client_no);
        exit(1);
    }
    else
        printf("Socket creation succes!\n");
    
    bzero(&client->serveraddr, sizeof(client->serveraddr)); 
    
    client->serveraddr.sin_family = AF_INET;
    client->serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	client->serveraddr.sin_port = htons(PORT(client_no)); 
    
    printf("Client %d has port %d", client_no, PORT(client_no));
    
    /*
     * Bind the created socket to the ip, port etc
     * */
    if(bind(client->socketfd, (struct sockaddr*)&client->serveraddr, sizeof(client->serveraddr)) !=0){
        printf("Socket bind fail for client %d...\n", client_no);
        exit(2);
    }else
        printf("Socket succesfully binded for client %d\n", client_no);
    
    
    /*
     * Server Listen for verification
     * */
    
    if(listen(client->socketfd, 2) != 0){
        printf("Listen fails...\n");
        exit(3);
    }
    else
        printf("ServerReady to accept client\n");
    
    /*
     * Link acknoledge
     * */
    client->len = sizeof(client->clientaddr);
    client->iofd=accept(client->socketfd, (struct sockaddr*)&client->clientaddr, &client->len);
    if(client->iofd < 0){
        printf("Accept failed...\n");
        exit(4);
    }
    else
        printf("Server accepted the cleint %d and generated the client input/output file descriptor\n", client_no);
    
    init_word[8] = client_no;
    write(client->iofd, init_word, sizeof(init_word));
}



int main(int argc, char**argv){
    struct clinet_descriptor clientX, client0;
    
    init_conversation(&clientX, CLIENT_NO_1);
    init_conversation(&client0, CLIENT_NO_2);
    
    
    
}
