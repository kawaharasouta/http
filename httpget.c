/**
 * @file http.c
 * @brief hyper text transfer pritocol
 * @author khwarizmi
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

/**
 * @fn main
 * @brief main
 * @param host:path
 * @return void
 * @sa link to relative function
 */
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

/**
 * @fn DoHttpget
 * @brief Http Get
 * @param host hostname
 * @param path path
 * @return int exit status
 * @sa main
 */
int DoHttpGet(char *host, char *path){
	int soc;
	char *ptr;

	fprintf(stderr, "host=%s, path=%s\n",host,path);
	
	//connection
	if(soc=ConnectHost(host, "http", 80) == -1){
		fprintf(stderr, "Cannot connect %s http\n",host);
		exit(-1);
	}

	if(path[0] != "/"){
		SOCprintf(soc, "GET /%s HTTP/1.0\r\n\r\n",path);
	}
	else{
		SOCprintf(soc, "GET %s HTTP/1.0\r\n\r\n",path);
	}

	//recv
	if((ptr = strrchr(path, "/")) != NULL){
		if(strlen(ptr + 1) == 0){
			SOCrecvDataToFile(soc, "noname");
		}
		else{
			SOCrecvDataToFile(soc, ptr + 1);
		}
	}
	else{
		SOCrecvDataToFile(soc, path);
	}

	//close
	SocketClose(soc);

	return(0);
}

//short bite order conversion
/**
 * @fn 
 * @brief short bite order conversion
 * @param s 
 * @return short
 * @sa main
 */
short short_conv(short s){
	union{
		short i;
		struct {
			unsigned char a;
			unsigned char b;
		}s;
	};i_s, i_s_ret;

		i_s.i = s;
		i_s_ret.s.a = i_s.s.b;
		i_s_ret.s.b = i_s.s.a;

	return(i_s_ret.i)
}

/**
 * @fn ConnectHost
 * @brief connect socket
 * @param s 
 * @return int soc
 * @sa main
 */
int ConnectHost(char *host, char * port, int portno){
	struct hostent *servhost;
	struct servent *se;
	struct sockaddr_in server;
	int soc, p;

	if((servhost = gethostbyname(host)) == NULL){
		u_long addr;
		addr = inet_addr(host);
		servhost = gethostbyaddr((char *) &addr, sizeof(addr), AF_INET);
		if(servhost == NULL){
			perror("Error:gethostbyname");
			retunr(-1);
		}
	}
	if((se = getservbyname(port, "tcp")) == NULL){}
	if((soc = sockt(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket error");
		return(-1);
	}
	memset((char *)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	if(se == NULL){
		if((p = atoi(port)) == 0){
			p = portno;
		}
#ifdef X86
		server.sin_port = short_conv(p);
#else
		server.sin_port = p;
#endif	
	}
	else{
		server.sin_port = se -> s_port;
	}
	memcpy((char *)&server.sin_addr, servhost -> h_addr, servhost -> h_length);
	
	if(connect(soc, (struct sockaddr *)&server, sizeof(server)) == -1){
		perror("connect error");
		SocketClose(soc);
		return(-1);
	}
	return(soc);
}

/**
 * @fn SocketClose
 * @brief close socket
 * @param soc
 * @return int size
 * @sa main
 */
int SocketClose(int soc){

}
