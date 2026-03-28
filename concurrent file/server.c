
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <fcntl.h>

#define PORT 4040
#define MAX 100

int main(){
	int serverSock, newServerSock, f, n;
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t addrlen;
	serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSock == -1){
		printf("Socket creation failed\n");
		exit(0);
	}
	else
		printf("Socket successfully created\n");
	bzero(&serverAddr, sizeof(serverAddr));
	char filename[MAX], filecontents[MAX], buffer[MAX];
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(PORT);
	
	if ((bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr))) != 0){
		printf("socket bind failed\n");
		exit(0);
	}
	else
		printf("Socket successfully binded\n");
	if ((listen(serverSock, 5)) != 0){
		printf("Listen failed\n");
		exit(0);
	}
	else
		printf("Server listening\n");
		
	while(1){
		addrlen = sizeof(clientAddr);
		newServerSock = accept(serverSock, (struct sockaddr*)&clientAddr, &addrlen);
		if(fork()==0){
			close(serverSock);
			pid_t pid = getpid();
			n = read(newServerSock, filename, 100);
			filename[n]='\0';
			f = open(filename, O_RDONLY);
			printf("Requested file is %s\n", filename);
			if(f<0){
				sprintf(buffer, "File not found\n");
				write(newServerSock, buffer, 100);	
			}else{
				n = read(f, filecontents, 100);
				filecontents[n] = '\0';
				printf("File contents are %s\n", filecontents);
				write(newServerSock, filecontents, 100);
			}
			sprintf(buffer, "Served by pid %d\n", pid);
			write(newServerSock, buffer, 100);
			exit(0);	
		}else{
			close(newServerSock);
		}
	}
	close(serverSock);
	return 0;
}




