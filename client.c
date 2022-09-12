#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
#include "struc.h"
#include "util.h"

int main(){
	
	int cfd = socket(AF_INET, SOCK_STREAM, 0);
	if(cfd == -1){
		printf("socket creation failed\n");
		exit(0);
	}
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port= htons(8086);
	server.sin_addr.s_addr =INADDR_ANY;      
	if(connect(cfd, (struct sockaddr *)&server, sizeof(server))==-1){
		printf("Connection Failed\n");
		exit(0);
	}
	printf("\t\t\tConnection Established\n\n");
	
	while(mainmenu(cfd)!=3);
	close(cfd);

	return 0;
}
