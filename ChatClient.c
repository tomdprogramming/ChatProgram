/*
 * ChatClient.c
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "tomIO.h"
 
#define BACKLOG 10
 
int main(int argc, char* argv[])
{
    struct addrinfo Hints;
    struct addrinfo* ServerInfo;
    struct addrinfo* p;
    char* message;
    char IncomingMessage[100];
    char* QuitMessage = "qqq";
    int ClientConnect = 1;
    int Status;
    int BytesSent, BytesReceived;
    char ipstr[INET6_ADDRSTRLEN];
    int SockFD;
    int ID;
	 
    if(argc != 1)//user has not used the command correctly
    {
        fprintf(stderr, "ChatClient doesn't take any command-line arguments!");
        return 1;
    }
 
    memset(&Hints, 0, sizeof(Hints));
    Hints.ai_family = AF_INET; //using IPv4
    Hints.ai_socktype = SOCK_STREAM; //using tcp
 
    //the address is filled in manually by knowing the ChatHost local host's address
    Status = getaddrinfo("192.168.0.3", "32000", &Hints, &ServerInfo);
	 
    if(Status!=0)
    {
        fprintf(stderr,"getaddrinfo() failed : %s\n\n", gai_strerror(Status));
        return 2;
    }

    void* Address;
    char* IPVersion;
     
    for(p=ServerInfo;p!=NULL;p=p->ai_next)
    {
        if(p->ai_family==AF_INET) //it's IPv4
        {
            struct sockaddr_in* IPv4Address;
            IPv4Address = (struct sockaddr_in*)p->ai_addr;
            Address = &(IPv4Address->sin_addr);
            IPVersion = "IPv4";
        }
        else //it's IPv6
        {
            struct sockaddr_in6* IPv4Address;
            IPv4Address = (struct sockaddr_in6*)p->ai_addr;
            Address = &(IPv4Address->sin6_addr);
            IPVersion = "IPv6";
        }
		 
        inet_ntop(p->ai_family, Address, ipstr, sizeof(ipstr));
        printf("Server Address is %s : %s\n\n", IPVersion, ipstr);
          
    }//end of for loop to loop through getaddrinfo returns
	 
    //create socket over which to communicate
    SockFD = socket(ServerInfo->ai_family, ServerInfo->ai_socktype, ServerInfo->ai_protocol);
    Status = connect(SockFD, ServerInfo->ai_addr, ServerInfo->ai_addrlen);
     
    if(Status!=0)
    {
        printf("connect() failed\n\n");
        return 3;
    }
	 
    printf("Opened connection!\n\n");
    int length;
    ID = fork();
	 
    if(ID>0)//I am the parent, I will send messages from the user to the host
    {	 
	    while(ClientConnect!=0)
        {
            printf("\nEnter message : ");
            message = GetString();
            ClientConnect = strcmp(message, QuitMessage);
            if(ClientConnect!=0)
            {
                printf("\nMessage to send is : %s\n", message);
                length = strlen(message);
                BytesSent = send(SockFD, message, length, 0);
                printf("%d bytes sent!", BytesSent);
                // memset(message, 0, 100);
            }
            else
            {
                printf("Closing connection...\n\n");
                BytesSent = send(SockFD, message, length, 0);
            }
            free(message);
        }
	 
        close(SockFD);
        freeaddrinfo(ServerInfo);
	 
        return 0;
    }
    else if(ID==0) // I am the child, I will listen for and display messages from other users
    {
        while(1)
        {
            memset(IncomingMessage,0,100);
            BytesReceived = recv(SockFD, message, 100, 0);
            printf("\n%d Bytes received!\n", BytesReceived);
            if(BytesReceived>0)
            {
                printf("\nMessage from host: %s\n", IncomingMessage);
            }
            sleep(1);
        }
    }
        
}//end of main()
