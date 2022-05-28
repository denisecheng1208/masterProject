#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>

#define MAX 255
int main(int argc, char *argv[]) {
	int sock;
	struct sockaddr_in servAddr;
	struct sockaddr_in fromAddr; 
	unsigned short servPort;
	unsigned int fromSize; 
	char *servIP; 
	char buffer[MAX+1]; 
	int respStringLen; 

	if (argc != 3) {
		printf("Usage: %s <Server IP> <server Port>\n", argv[0]);
		exit(1);
	}
	
	servIP = argv[1];
	servPort = atoi(argv[2]);

	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		printf("socket() failed.\n"); 

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(servIP);
	servAddr.sin_port = htons(servPort);
	printf("Connected to the server: %s port: %d.\n",servIP, servPort);
	if(connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0){
		printf("connect() failed.\n");
		exit(1);
	}

	if ((respStringLen = recv(sock, buffer, MAX, 0)) < 0) {
		 printf("recv() failed.\n");
		 exit(1);
    }

	printf("Finished receiving.\n");
	//if (respStringLen ==)
	printf("buffer: %s\n",buffer);
    close(sock);

	sleep(50);
	system("./runclient.sh");	
}
