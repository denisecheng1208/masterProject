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
	char *string; 
	int stringLen; 
	char buffer[MAX+1]; 
	int respStringLen; 

	if (argc != 4) {
		printf("Usage: %s <Server IP> <server Port>\n", argv[0]);
		exit(1);
	}
	
	servIP = argv[1];
	servPort = atoi(argv[2]);
	string = argv[3];
	stringLen = strlen(string);

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

	if(send(sock, string, stringLen, 0) != stringLen){
		printf("send() failed.\n");
		exit(1);
	}
	printf("Finished sending request.\n");

	if ((respStringLen = recv(sock, buffer, MAX, 0)) < 0) {
		 printf("recv() failed.\n");
		 exit(1);
    }

	printf("Finished receiving.\n");
	printf("buffer: %s\n",buffer);
    close(sock);

	if (strcmp(string, "open")==0 && strcmp(buffer, "OK")==0) {
		printf("wait 50s to open flightgear\n");
		sleep(50);
		system("./runclient.sh");	
	}
}
