#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<time.h>
void main(){
	int sockfd;
	struct sockaddr_in server;
	socklen_t len = sizeof(server);
	char buffer[1024];
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0){
		perror("Socket creation failed!!!");
		exit(1);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(5173);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	while(1){
		strcpy(buffer,"TIME :");
		sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr *)&server,len);
		recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&server,&len);
		printf("%s",buffer);
		sleep(5);
	}
	close(sockfd);
	return;
}
