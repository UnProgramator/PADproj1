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
#define CLIENT_NO_X 0
#define CLIENT_NO_0 1

struct clinet_descriptor{
    int iofd; //file descriptor to be used with read and write
    int socketfd; //socket file clinet_descriptor
    int len; //client data
    struct sockaddr_in serveraddr; //server data
    struct sockaddr_in clientaddr; //client data
};

void init_conversation(struct clinet_descriptor *client, const char client_no){
    static unsigned char init_word[] = {0, 0, 0, 
                                        0, 0, 0, 
                                        0, 0, 0, 
                                                 0};
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
    
    printf("Client %d has port %d\n", client_no, PORT(client_no));
    
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
        printf("Server accepted the client %d and generated the client input/output file descriptor\n", client_no);
    
    init_word[9] = client_no+1;
    write(client->iofd, init_word, sizeof(init_word));
}

/*
 * 0 1 2
 * 3 4 5
 * 6 7 8
 */


int has_win(char v[10]){
    int i;
    /*
     * Verific liniile si coloanele si diagonalele
     * Daca gasesc ca exista o "linie" cu aceasi valoare, returnez acea valoare
     */
    if(v[0] && 
        (v[0]==v[1]==v[2] || 
         v[0]==v[4]==v[8] ||
         v[0]==v[3]==v[6]))
        return v[0];
    if(v[4] &&
        (v[1]==v[4]==v[7] ||
         v[3]==v[4]==v[5] ||
         v[2]==v[4]==v[6]))
        return v[4];
    if(v[8] &&
        (v[6]==v[7]==v[8] ||
         v[2]==v[5]==v[8]))
        return v[8];
    
    /*
     * Daca mai exista spatii cu 0 atunci se mai pot face miscari
     * Altfel returnez codul pentru remiza care este 3
     */
    for(i=0; i<9; i++)
        if(v[i]==0);
            return 0;
    return 3;
}

void play_game(int player1fd, int player2fd){
    char cuvant[10];
    int i;
    
    bzero(cuvant, sizeof(cuvant));
    
    while(1){
        read(player1fd, cuvant, sizeof(cuvant));
        
        printf("Player X has made the move: \n");
        for(i=0; i<9; i++){
            printf("%d ", cuvant[i]);
            if(i%3==2) printf("\n");
        }
        printf("\t\t\t%d\n", cuvant[9]);
        
        cuvant[9] = has_win(cuvant);
        
        if(cuvant[9]){
            printf("Player X won!\n");
            goto win_tag;
        }
        
        write(player2fd, cuvant, sizeof(cuvant));
        
        read(player2fd, cuvant, sizeof(cuvant));
        
        printf("Player Y has made the move: \n");
        for(i=0; i<9; i++){
            printf("%d ", cuvant[i]);
            if(i%3==2) printf("\n");
        }
        printf("\t\t\t%d\n", cuvant[9]);
        
        cuvant[9] = has_win(cuvant);
        
        if(cuvant[9]){
            printf("Player Y won!\n");
            goto win_tag;
        }
        
        
        write(player1fd, cuvant, sizeof(cuvant));
    }
    win_tag:
        write(player1fd, cuvant, sizeof(cuvant));
        write(player2fd, cuvant, sizeof(cuvant));
}

int main(int argc, char**argv){
    struct clinet_descriptor clientX, client0;
    //while(1){
        init_conversation(&clientX, CLIENT_NO_X);
        init_conversation(&client0, CLIENT_NO_0);
        
        /*
        * Prin conventie am ales "X" -> player 1 si "0" -> player 2
        */
        
        play_game(clientX.iofd, client0.iofd);
        
        close(clientX.iofd);
        close(client0.iofd);
        close(clientX.socketfd);
        close(client0.socketfd);
        bzero(&clientX, sizeof(clientX));
        bzero(&client0, sizeof(client0));
        printf("\nReset players\n\n");
        sleep(1);
    //}
    return 0;
    
}
