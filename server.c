#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include "struc.h"
#include "serutil.h"

int main(){
	
	struct sockaddr_in server, client;
	int sd, c, nsd;
	
	sd = socket(AF_INET, SOCK_STREAM,0);
        
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8086);
	printf("Connecting to Client...\n");

	if(bind(sd,(struct sockaddr *)(&server), sizeof(server))<0){
		printf("Error at binding\n");
	}

	listen(sd,10);
	c = sizeof(client);
	while(1){
		nsd = accept(sd, (struct sockaddr *)&client, &c);
		if(!fork()){
			close(sd);
			mainmenu(nsd);
			exit(0);
		}
		else
		close(nsd);
		

	}
		
	close(nsd);
	close(sd);
	return 0;
}

