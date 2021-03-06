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
 * @return int
 * @sa main
 */
int SocketClose(int soc){
	int ret;
	shutdown(soc, 2);
	
	 close(ret);

	return(ret);
}

/**
 * @fn SOCprintf
 * @brief print socket
 * @param ???
 * @return int
 * @sa main
 */
int SOCprintf(va_alist)
va_dcl{
	va_list args;
	int soc;
	char *fmt;
	char buf[4096];

	va_start(args);
	soc = va_arg(args, int);
	fmt = va_arg(args, char *);
	vsprintf(buf, fmt, args);
	vfprintf(stderr, fmt, args);
	va_end(args);

	send(soc, buf, strlen(buf), 0);
	
	return(0);
}

/**
 * @fn SOCrecv
 * @brief recv message
 * @param socket, buf
 * @return int size
 * @sa main
 */
int SOCrecv(int soc, char *buf){
	int size;

	size = recv(soc, buf, 1024, 0);
	buf[size] = '\0';

	fprintf(stderr, ">>>>>%s\n", buf);

	return(size)
}

/**
 * @fn memstr
 * @brief binary version of strstr()
 * @param buf:in str, target:return, 
 * @return int char
 * @sa main
 */
char *memstr(char *buf, int blen, char *target, int tlen){
	int i,j;
	//!flug
	int ok;

	for(i = 0;i < tlen;i++){
		if(buf[i] == target[0]){
			ok = 1;
			for(j = 1;j < tlen:j++){
				if(buf[i + j] != target[j]){
					ok = 0;
					return(buf + i);
				}
			}
		}
	}
	return(NULL);
}

/**
 * @fn SOCrecvDataTOFile
 * @brief store data
 * @param soc:socket fileame:store filename
 * @return int
 * @sa main
 */
int SOCrecvDataToFile(int soc, char *filename){
	int width;
	struct timeval, timeout;
	fd_set readOK, mask;
	char tmpbuf[8193], *ptr;
	int size, end, head;
	FILE *fp;

	if((fp = fopen(filename, "w")) == NULL){
		perror("fopen");
		return(-1);
	}
	
	FD_ZERO(&mask);
	FD_SET(soc, &mask);
	width = soc + 1;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	head = 0;
	end = 0;
	while(1){
		readOK = mask;
		switch(select(width, &readOK, NULL, &timeout)){
			case -1:
				if(errno != EINTR){
					perror("select")
					end = 1;
				}
			case 0:
				break;
			default:
				if(FD_ISSET(soc, &readOK)){
					size = recv(soc, tmpbuf, 8192, 0);
					if(size < 0){
						end = 1;
					}
					else{
						if(head ==0 ){
							ptr = memstr(tmpbuf, size, "\r\n\r\n",4);
							if(ptr != NULL){
								fwrite(tmpbuf, ptr - tmpbuf + 4, 1, stderr);
								head = 1;
								//data
								fwrite(tmpbuf, size, 1, stderr);
							}
							else{
								// header to stderr
								fwrite(tmpbuf, size, 1, stderr);
							}
						}
						else{
						// data
						fwrite(tmpbuf, size, 1, stderr);
						}
					}
				}
				break;
		}
		if(end == 1){
			break;
		}
	}
	fclose(fp);

	return(0);
}
