#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define MAX 80
#define PORT 8990

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, client;
    socklen_t len = sizeof(client);
    char buff[MAX];
    srand(time(0));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
      perror("Socket creation failed"); 
      exit(1); 
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed"); 
        exit(1);
    }
    listen(sockfd, 5);
    printf("Server listening on port %d\n", PORT);
    connfd = accept(sockfd, (struct sockaddr *)&client, &len);
    if (connfd < 0) { 
      perror("Accept failed"); 
      exit(1); 
    }
    printf("Client connected\n");

    int expected = 0;       
    int last_ack = -1;    
    int ack_lost_done = 0;  
    while (1) {
        bzero(buff, MAX);
        int n = recv(connfd, buff, MAX, 0);
        if (n <= 0) break;
        if (strcmp(buff, "finish") == 0) {
            printf("Transmission finished\n");
            break;
        }
        int frame = atoi(buff);
        if (frame == expected) {
            if (!ack_lost_done) {
                printf("Received packet %d → ACK LOST (simulated)\n", frame);
                ack_lost_done = 1;
                last_ack = frame;
                continue; 
            }
            printf("Received packet %d → ACK sent\n", frame);
            sleep(2);
            last_ack = frame;
            expected++;
        } else if (frame <= last_ack) {
            printf("Duplicate Frame %d → ACK resent\n", frame);
        } else {
            printf("Unexpected Frame %d received\n", frame);
        }
        snprintf(buff, sizeof(buff), "%d", frame + 1);
        send(connfd, buff, MAX, 0);
    }

    close(connfd);
    close(sockfd);
    return 0;
}


