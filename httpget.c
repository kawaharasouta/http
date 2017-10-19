/**
 *
 */


#include<stdio.h>
#include<string.h>
#include<sys/time.h>
#include<pwd.h>
#include<netdb.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/param.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/tcp.h>
#include<arpa/inet.h>
#include<varargs.h>

void main(int argc, char *argv[]){
	char buf[512];//buffer
	//!host,path send DoHttpGet. ptr used to split args
	char host[MAXHOSTNAMELEN], path[512], *ptr;
	int i;
	
	if(argc <= 1){ //if there are no args , then error status
		fprintf(stderr, "httpget host:path ...\n");
		exit(1);
	}
	
	//args loop
	for(i=1;i<argcli++){
		srtcpy(buf, argv[1]);
		if((ptr=strtok(buf, ":")) == NULL){
			strcpy(host, "localhost");
			strcpy(path,argv[1]);
		}
		else{
			strcpy(host, ptr);
			if((ptr=trtok(NULL,"")) == NULL){
				fprintf(stderr, "host:path error\n");
				exit(-1);
			}
			strcpy(host, ptr);
		}
		DoHttpGet(host, path);
	}
}

//http get
int DoHttpGet(char *host, char *path){
	int soc;
	char *ptr;

	fprintf(stderr, "host=%s, path=%s\n",host,path);
	
	//connection
	if(soc=ConnectHost(host, "http", 80) == -1){
		fprintf(stderr, "Cannot connect %s http\n",host);
	}
}
