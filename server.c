#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>

#define MAX 255

int main(int argc, char *argv[]){
	int sock;
	struct sockaddr_in severAddr;
	struct sockaddr_in clientAddr;
	unsigned int cliAddrLen;
	char recvBuffer[MAX] = "";
	unsigned short severPort;
	int recvMsgSize;

	char *serverVMs[] = {"server1", "server2"};
	char *vmIPs[] = {"192.168.56.103", "192.168.56.104"};
	
	if(argc!=2){
		printf("Usage: %s <TCP PORT>\n", argv[0]);
	}

	severPort = atoi(argv[1]);
	if((sock = socket(PF_INET,SOCK_STREAM,0))<0){
		printf("socket() failed.\n");
		exit(1);
	}

	memset(&severAddr, 0, sizeof(severAddr));
	severAddr.sin_family = AF_INET;
	severAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	severAddr.sin_port = htons(severPort);
	if(bind(sock,(struct sockaddr *) &severAddr, sizeof(severAddr)) < 0){
		printf("bind() failed.\n");
		exit(1);
	}
	listen(sock, 1);

	for( ; ; ){
		int newsockfd = 0;
		cliAddrLen = sizeof(clientAddr);
		if((newsockfd = accept(sock, (struct sockaddr *) &clientAddr, &cliAddrLen)) < 0){//create a new socket
			printf("accept() failed.\n");
			exit(1);
		}

		printf("**************************************************************\n");
     	printf("Accept client %s on TCP port %d.\n",inet_ntoa(clientAddr.sin_addr), clientAddr.sin_port);

	 	char *reply = "OK";
		int replyLen = strlen(reply);
        printf("Send Reply...\n");
        if(send(newsockfd, reply, replyLen, 0) != replyLen){
			printf("send() failed.\n");
			exit(1);
        }

		char cmd[50];
		sprintf(cmd, "./runserver.sh %s %s", serverVMs[0], inet_ntoa(clientAddr.sin_addr));
		printf("cmd: %s.\n", cmd);
		system(cmd);
		//system("BREAK");
		// sleep(30);

        printf("End communication ...\n");
		close(newsockfd);
    }
}
