#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/socket.h>
#include <stdlib.h>

#define PORT 4040
#define MAX 100


int main(){
	int clientSock;
	struct sockaddr_in serverAddr;
	socklen_t addrlen;
	char buffer[MAX], filename[MAX];
	
	clientSock = socket(AF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(PORT);
	
	if (connect(clientSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0){
		printf("Connection with the server failed\n");
		exit(0);
	}else 
		printf("Connected to the server\n");
	printf("Enter filename : ");
	scanf("%s", filename);
	write(clientSock, filename, strlen(filename));
	int n=read(clientSock, buffer, 100);
	buffer[n] = '\0';
	printf("Response from server:\n%s\n",buffer); 
	n=read(clientSock, buffer, 100);
	buffer[n] = '\0';
	printf("%s\n",buffer);
	close(clientSock);
	return 0;
}



