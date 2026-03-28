#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 32
#define PORT 5173

void send_frames(int sockfd, int nf, int ws)
{
    char buff[MAX];
    int base = 0, next = 0, ack;

    struct timeval timeout = {3, 0};
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    /* Initial window */
    while (next < nf && next < ws)
    {
        snprintf(buff, MAX, "%d", next);
        send(sockfd, buff, strlen(buff) + 1, 0);
        printf("Frame %d sent\n", next++);
    }

    while (base < nf)
    {
        memset(buff, 0, MAX);
        if (recv(sockfd, buff, MAX, 0) > 0)
        {
            ack = atoi(buff);
            if (ack >= base)
            {
                base = ack + 1;
                printf("Acknowledgement received: %d\n", ack);
            }
        }
        else
        {
            printf("Acknowledgement not received for %d → Resending frames\n", base);
            next = base;
        }

        while (next < nf && next < base + ws)
        {
            snprintf(buff, MAX, "%d", next);
            send(sockfd, buff, strlen(buff) + 1, 0);
            printf("Frame %d sent\n", next++);
        }
    }

    strcpy(buff, "Exit");
    send(sockfd, buff, strlen(buff) + 1, 0);
}

int main(void)
{
    int sockfd, frames, window;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Socket failed");
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("Connected to server\n");

    printf("Enter number of frames: ");
    scanf("%d", &frames);

    printf("Enter window size: ");
    scanf("%d", &window);

    send_frames(sockfd, frames, window);

    close(sockfd);
    return 0;
}


