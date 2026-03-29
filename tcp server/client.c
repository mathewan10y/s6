#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 5173
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in server_addr;
    fd_set readfds;
    char buffer[BUFFER_SIZE];
    char username[50];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket created...\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    server_addr.sin_addr.s_addr = inet_addr("172.18.17.49");

    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Connected to server on port : %d\n", PORT);

    printf("Enter username: ");
    fgets(username, 50, stdin);
    username[strcspn(username, "\n")] = 0;
    send(sockfd, username, strlen(username), 0);

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);
        FD_SET(sockfd, &readfds);

        select(sockfd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(0, &readfds)) // when the client enters a message and presses enter, it becomes ready to read from stdin(0)
        {
            fgets(buffer, BUFFER_SIZE, stdin);
            send(sockfd, buffer, strlen(buffer), 0);
        }

        if (FD_ISSET(sockfd, &readfds)) // when the server sends a message to the client, it becomes ready to read from sockfd
        {
            int bytes = read(sockfd, buffer, BUFFER_SIZE);
            if (bytes <= 0)
                break;
            buffer[bytes] = '\0';
            printf("%s", buffer);
        }
    }

    close(sockfd);
    return 0;
}
