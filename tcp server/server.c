#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/select.h>

#define PORT 5173
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

int main()
{
    int sockfd,newfd,client_sockets[MAX_CLIENTS];
    struct sockaddr_in server_addr,client_addr;
  
    /*struct sockaddr_in
      {
    short sin_family;
    unsigned short sin_port;
    struct in_addr sin_addr;
    char sin_zero[8];
        } */
    //struct in_addr is another predefined structure used to store an IPv4 address inside sockaddr_in.
    socklen_t len;
    fd_set readfds;//fd_set is a special structure used by select() to store a set of file descriptors.
    char buffer[BUFFER_SIZE];
    char usernames[MAX_CLIENTS][50];

    for(int i=0;i<MAX_CLIENTS;i++)
    {
        client_sockets[i]=0;
        memset(usernames[i],0,50);
    }

    sockfd=socket(AF_INET,SOCK_STREAM,0);//The OS creates a socket and returns an integer no-a file descriptor(it starts from 3). ie- server=3, client1-2,etc.
    printf("Socket created...\n");//socket=communication endpoint

    //configure server address
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_port=htons(PORT);
    
    
    bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    //bind() is a system call used in socket programming to attach a socket to a specific IP address and port number.
    // syntax-bind(socket,pointer to addr,socketlength)
    // sockaddr_in is typecast to sockaddr
    printf("Binding success...\n");

    listen(sockfd,5);//listen() converts a bound socket into a listening socket. 5(backlog) is the maximum connection requests that can wait in the queue.
    printf("Listening on port : %d\n",PORT);

    while(1)
    {
        FD_ZERO(&readfds); //Clears the set.
        FD_SET(sockfd,&readfds);//Adds a descriptor to the set.(here,server)
        int maxfd=sockfd;

        for(int i=0;i<MAX_CLIENTS;i++)
        {
            int sd=client_sockets[i];
            if(sd>0) FD_SET(sd,&readfds);//add clients to the set
            if(sd>maxfd) maxfd=sd;
        }

        select(maxfd+1,&readfds,NULL,NULL,NULL);  //select() waits until one of the sockets has activity.(New client tries to connect/Existing client sends a message/ Client disconnects)
        //syntax: select(maxfd+1,&readfds,NULL,NULL,NULL);
        
        //After select() returns, readfds contains only the sockets that have activity.

        if(FD_ISSET(sockfd,&readfds))//fd_isset Checks if a descriptor has activity.
        //here it checks whether a particular socket is ready after select().(it becomes true when client tries to connect)
        {
            len=sizeof(client_addr);
            newfd=accept(sockfd,(struct sockaddr*)&client_addr,&len);// accept() fills client_addr and returns a file descriptor of the client

            for(int i=0;i<MAX_CLIENTS;i++)
            {
                if(client_sockets[i]==0)//  searches for an empty slot in the array to enter new client
                {
                    client_sockets[i]=newfd;
                    read(newfd,usernames[i],50);//Read the client's username  that it sends using 'send'
                    printf("Client %s@%s\n",
                           usernames[i],inet_ntoa(client_addr.sin_addr)); //inet_ntoa is used to convert a binary IP address into a human-readable string.
                    break;
                }
            }
        }

        for(int i=0;i<MAX_CLIENTS;i++)
        {
            int sd=client_sockets[i];
            if(sd>0 && FD_ISSET(sd,&readfds)) // checks if valid cleint id and if client sent data
            {
                int bytes=read(sd,buffer,BUFFER_SIZE);  //reads from client data( number of bytes received is stored in bytes)
                if(bytes<=0)
                {
                    printf("Client disconnected : %s\n",usernames[i]);
                    close(sd);
                    client_sockets[i]=0;
                    memset(usernames[i],0,50);
                }
                else
                {
                    buffer[bytes]='\0';
                    printf("Message from %s : %s",usernames[i],buffer);

                    char msg[BUFFER_SIZE+100];
                    sprintf(msg,"%s : %s",usernames[i],buffer);

                    for(int j=0;j<MAX_CLIENTS;j++)
                    {
                        if(client_sockets[j]>0 && client_sockets[j]!=sd)
                            send(client_sockets[j],msg,strlen(msg),0);
                    }
                }
            }
        }
    }
    return 0;
}



