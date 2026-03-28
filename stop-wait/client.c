#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define MAX 80
#define PORT 8990

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buff[MAX];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { 
      perror("Socket creation failed"); 
      exit(1);
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
        perror("Connection with server failed"); 
        exit(1);
    }
    printf("Connected to server\n");
    int nframes;
    printf("Enter number of packet: ");
    scanf("%d", &nframes);
    struct timeval timeout = {2, 0};
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    for (int i = 0; i < nframes; ) {
        snprintf(buff, sizeof(buff), "%d", i);
        send(sockfd, buff, MAX, 0);
        printf("packet %d sent\n", i);
        bzero(buff, MAX);
        int n = recv(sockfd, buff, MAX, 0);
        if (n > 0) {
            int ack = atoi(buff);
            if (ack == i + 1) {
                printf("Acknowledgement received: %d\n", ack);
                i++; 
            }
        } else {
            printf("ACK for packet %d not received → Resending\n", i);
        }
    }
    snprintf(buff, sizeof(buff), "finish");
    send(sockfd, buff, MAX, 0);

    close(sockfd);
    return 0;
}


