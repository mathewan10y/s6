#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<time.h>
#include<unistd.h>

void main(){
	int sockfd;
	struct sockaddr_in server,client;
	socklen_t len = sizeof(client);
	char buffer[1024];
	fd_set readfds;
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0){
		perror("Socket creation failed!!!");
		exit(1);
	}
	printf("Socket creation successful!!!\n");
	server.sin_family = AF_INET;
	server.sin_port = htons(5173);
	server.sin_addr.s_addr = INADDR_ANY;
	if(bind(sockfd,(struct sockaddr *)&server,sizeof(server))<0){
		perror("Binding failed!!!");
		exit(1);
	}
	printf("Running on Port : 5173\n");
	while(1){
		FD_ZERO(&readfds);
		FD_SET(sockfd,&readfds);
		select(sockfd+1,&readfds,NULL,NULL,NULL);
		if(FD_ISSET(sockfd,&readfds)){
			recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&client,&len);
			time_t now;
			time(&now);
			snprintf(buffer,sizeof(buffer)-1," Server Time : %s",ctime(&now));
			sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr *)&client,len);
		}
	}
	close(sockfd);
	return;
}
