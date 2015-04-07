/*
** ChatProgram.c - connect two computers to be able to send simple text messages
* The system will be that one computer runs as a host, and the other runs as a client.
* This is the host program
* I'm going to arbitrarily choose to run the service on port 32000
* 
* This is a learning exercise
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BACKLOG 10

int main(int argc, char* argv[])
{
	//declarations
	struct sockaddr_storage ClientAddress;
	struct addrinfo Hints;
	struct addrinfo* ServerInfo;
	struct addrinfo* p;
	int SockFD;
	int NewSockFD;
	socklen_t addr_size;
	int Status;
    char hostname[64];
	char ipstr[INET6_ADDRSTRLEN];
	char message[100];
	memset(message, 0, 100);
	int BytesReceived;
	int id;
	int ClientConnected = 1;
	char* QuitMessage = "qqq";
	
	gethostname(hostname, 64);
	
	printf("hostname is : %s\n\n",hostname);
	
	//set up the addrinfo struct for a listening server
	
	//first make sure that hints starts out completely empty so that getaddrinfo() doesn't get any bogus hints
	memset(&Hints, 0, sizeof(Hints));
	
	//fill out hints
	Hints.ai_family = AF_UNSPEC; //we don't care what IP protocol we use
	Hints.ai_socktype = SOCK_STREAM; //this is a chat client, we're gonna need to make sure our message arrives complete and in order
	Hints.ai_flags = AI_PASSIVE; //the sockets functions can fill in my IP address for me
	
	//Status = getaddrinfo("tea-desktop.teahouse", "32000", &Hints, &ServerInfo);
	Status = getaddrinfo("192.168.0.3", "32000", &Hints, &ServerInfo);
	
	if(Status != 0)//system unable to assign any addresses
	{
		fprintf(stderr, "getaddrinfo error : %s\n\n", gai_strerror(Status));//give the error message to the stderr stream to pass to the user
		return 1;//exit program
	}
	
	void* Address;
	char* IPVersion;
	
	for(p=ServerInfo;p!=NULL;p=p->ai_next)
	{
		
		if(p->ai_family == AF_INET)//address is IPv4
		{
			struct sockaddr_in* IPv4Address;
			IPv4Address = (struct sockaddr_in*)p->ai_addr;
			Address = &(IPv4Address->sin_addr);
			IPVersion = "IPv4";
		}
		else //address is IPv6
		{
			struct sockaddr_in6* IPv6Address;
			IPv6Address = (struct sockaddr_in6*)p->ai_addr;
			Address = &(IPv6Address->sin6_addr);
			IPVersion = "IPv6";
		}
	
		//convert the IP from a number to a string so it can be printed in user readable format
		inet_ntop(p->ai_family, Address, ipstr, sizeof ipstr);
		printf("server address is %s : %s\n\n", IPVersion, ipstr);
	}
	
	
	//create socket to listen on
	SockFD = socket(ServerInfo->ai_family, ServerInfo->ai_socktype, ServerInfo->ai_protocol);
	bind(SockFD, ServerInfo->ai_addr, ServerInfo->ai_addrlen);
	printf("Waiting for client to connect...\n\n");
	
	addr_size = sizeof(ClientAddress);
	
	listen(SockFD, BACKLOG);//listen for incoming connection
	
	//accept returns a new socket FD, that we can use in future send and recieve	
	NewSockFD = accept(SockFD, (struct sockaddr*)&ClientAddress, &addr_size);
	
	id = fork();
	if(id==-1)
	{
		//fork failed
	}
	else if(id>0)//I am the parent, I wil go right back to listening for new incoming connections and leave the child to deal with the new client
	{
		
	}
	else if(id==0)//I am the child, I will handle communication on this socket 
	{
		printf("A client has connected!!!\n\n");
	
		while(ClientConnected != 0)
		{
			BytesReceived = recv(NewSockFD,message,100,0);
	
			printf("%d Bytes Received!!!\n\n",BytesReceived);
		
			if(BytesReceived>0)
			{
				ClientConnected = strcmp(message, QuitMessage);
				printf("ClientConnect value is now %d\n", ClientConnected);
				if(ClientConnected == 0)
				{
					printf("Connection closed by client\n");
				}
				else
				{
					printf("message from other machine : %s\n", message);
				}
			}
			else
			{
				printf("Client has disconnected unexpectedly\n\n");
				ClientConnected=0;
			}
			memset(message,0,100);
		}
	
	
		close(NewSockFD);
	}
	
	
	close(SockFD);
	freeaddrinfo(ServerInfo);
	
	return 0;
}


