#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 32
#define PORT 5173

void handle_client(int connfd)
{
    char buff[MAX];
    int frame, ack = -1, expected = 0;

    while (1)
    {
        memset(buff, 0, MAX);
        if (recv(connfd, buff, MAX, 0) <= 0)
            break;

        if (strcmp(buff, "Exit") == 0)
        {
            printf("Exit\n");
            break;
        }

        frame = atoi(buff);

        /* Go-Back-N logic */
        if (frame != expected)
        {
            printf("Frame %d discarded\nAcknowledgement sent: %d\n", frame, ack);
        }
        else
        {
            if (rand() % 3 != 0)   // simulate loss
            {
                ack = frame;
                expected++;
                printf("Frame %d received\nAcknowledgement sent: %d\n", frame, ack);
            }
        }

        snprintf(buff, MAX, "%d", ack);
        send(connfd, buff, strlen(buff) + 1, 0);
    }
}

int main(void)
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    socklen_t len = sizeof(cli);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Socket failed");
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(sockfd, 5);

    printf("Server listening...\n");
    connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
    printf("Client connected\n");

    handle_client(connfd);

    close(connfd);
    close(sockfd);
    return 0;
}



