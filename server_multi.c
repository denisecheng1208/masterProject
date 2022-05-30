#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>

#define MAX 255

struct serverNode
{
	char *vmName;
	char *vmIP;
	struct serverNode *next;
};

struct serverList
{
	int size;
	struct serverNode *head;
	struct serverNode *tail;
};


void initServerList(struct serverList *list) {
	char *serverVMs[] = {"server1", "server2", "server3", "server4", "server5"};
	char *vmIPs[] = {"192.168.56.103", "192.168.56.104", "192.168.56.105", "192.168.56.106", "192.168.56.107"};
    int n = 5;

    struct serverNode *head = (struct serverNode *)calloc(1, sizeof(*head));
    head->vmName = serverVMs[0];
    head->vmIP = vmIPs[0];
    head->next = NULL;

    struct serverNode *p = head;
	struct serverNode *q;
	for (int i = 1; i < 5; i++){
        q = (struct serverNode *)calloc(1, sizeof(*q));
        q->vmName = serverVMs[i];
        q->vmIP = vmIPs[i];
        q->next = NULL;

        p->next = q;
        p = q;
    }

	list->size = n;
	list->head = head;
	list->tail = p;
}

struct serverNode *deleteFromHead(struct serverList *list) {
	struct serverNode *outNode = list->head;

	if (list->size == 1) {
		list->head = NULL;
		list->tail = NULL;
	}
	else {
		list->head = outNode->next;
		outNode->next = NULL;
	}

	list->size -= 1;
	return outNode;
}

void addToTail(struct serverList *list, struct serverNode *newNode) {
	if (list->size < 1) {
		list->head = newNode;
		list->tail = newNode;
	}
	else {
		list->tail->next = newNode;
		newNode->next = NULL;
		list->tail = newNode;
	}

	list->size += 1;
}

void printList(struct serverList *list) {
	struct serverNode *p = list->head;

	while (p != NULL)
	{
		printf("%s -> ", p->vmName);
		p = p->next;
	}
	printf("NULL\n");
	
}

int main(int argc, char *argv[]){
	int sock;
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	unsigned int cliAddrLen;
	char recvBuffer[MAX] = "";
	unsigned short serverPort;
	int recvMsgSize;
	struct serverList *inactiveList; // idle servers
	struct serverList *activeList; //occupied servers

	inactiveList = (struct serverList *)calloc(1, sizeof(*inactiveList));
	initServerList(inactiveList); 

	activeList = (struct serverList *)calloc(1, sizeof(*activeList));
	activeList->size = 0;
	activeList->head = NULL;
	activeList->tail = NULL;

	//test add and delete
	/*
	struct serverNode *assignedServer = deleteFromHead(inactiveList);
	addToTail(activeList, assignedServer);*/

	//test printList
	/*
	printf("Inactive Server List: ");
	printList(inactiveList);
	printf("Active Server List: ");
	printList(activeList);*/


	if(argc!=2){
		printf("Usage: %s <TCP PORT>\n", argv[0]);
	}

	serverPort = atoi(argv[1]);
	if((sock = socket(PF_INET,SOCK_STREAM,0))<0){
		printf("socket() failed.\n");
		exit(1);
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(serverPort);
	if(bind(sock,(struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0){
		printf("bind() failed.\n");
		exit(1);
	}
	listen(sock, 1);

	for( ; ; ){
		printf("Inactive Server List: ");
		printList(inactiveList);
		printf("Active Server List: ");
		printList(activeList);

		int newsockfd = 0;
		cliAddrLen = sizeof(clientAddr);
		if((newsockfd = accept(sock, (struct sockaddr *) &clientAddr, &cliAddrLen)) < 0){//create a new socket
			printf("accept() failed.\n");
			exit(1);
		}

		printf("**************************************************************\n");
     	printf("Accept client %s on TCP port %d.\n",inet_ntoa(clientAddr.sin_addr), clientAddr.sin_port);

		if (inactiveList->size > 0) {
			char *reply = "OK";
			int replyLen = strlen(reply);
			printf("Send Reply...\n");
			if(send(newsockfd, reply, replyLen, 0) != replyLen){
				printf("send() failed.\n");
				exit(1);
			}

			char cmd[50];
			// assign a server, add it to active list
			struct serverNode *assignedServer = deleteFromHead(inactiveList);
			addToTail(activeList, assignedServer);

			sprintf(cmd, "./runserver.sh %s %s %s", assignedServer->vmName, assignedServer->vmIP, inet_ntoa(clientAddr.sin_addr));
			printf("cmd: %s.\n", cmd);

			system(cmd);
		}
		else {
			char *reply = "Fail";
			int replyLen = strlen(reply);
			printf("Send Reply...\n");
			if(send(newsockfd, reply, replyLen, 0) != replyLen){
				printf("send() failed.\n");
				exit(1);
			}
		}

        printf("End communication ...\n");
		close(newsockfd);
    }
}
