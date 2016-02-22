#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<stdlib.h>
typedef struct sockaddr_in sckadr;
int main(int argc,char **argv)
{
	
	int port[argc-1];
	for(int i=1;i<argc;i++)
	port[i-1]=atoi(argv[i]);
	int sfd[argc-1];
	for(int i=1;i<argc;i++)
	{
		sfd[i-1]=socket(AF_INET,SOCK_STREAM,0);
		if(sfd[i-1]<0)
		printf("socket %d failed\n",i);
	}
	sckadr seradr[argc-1];
	for(int i=1;i<argc;i++)
	{
		seradr[i-1].sin_family=AF_INET;
		seradr[i-1].sin_port=htons(port[i-1]);
		seradr[i-1].sin_addr.s_addr=htonl(INADDR_ANY);
		bzero(seradr[i-1].sin_zero,8);
	}
	int m;
	for(int i=1;i<argc;i++)
	{
		m=bind(sfd[i-1],(struct sockaddr *)&seradr[i-1],sizeof(seradr[i-1]));
		if(m<0)
		printf("bind %d failed\n",i);
	}
	socklen_t clen[argc-1];
	sckadr cadr[argc-1];
	int n;
	for(int i=1;i<argc;i++)
	{
		n=listen(sfd[i-1],1);
		if(n<0)
		printf("listen %d failed\n",i);
	}
	int pid;
	char *buf=new char[1024];
	fd_set readset;
	struct timeval t;
	t.tv_sec=0;
	t.tv_usec=0;
	while(1)
	{
		FD_ZERO(&readset);
		for(int i=1;i<argc;i++)
		FD_SET(sfd[i-1],&readset);
		while(select(argc+2,&readset,NULL,NULL,&t)>0)
		{		
			for(int i=1;i<argc;i++)
			{
				if(FD_ISSET(sfd[i-1],&readset))
				{
					int nsfd=accept(sfd[i-1],(struct sockaddr *)&cadr[i-1],&clen[i-1]);
					if(nsfd>=0)
					{
						printf("entered\n");
						pid=fork();
						if(pid==0)
						{
							char *buf2=new char[100];
							for(int j=1;j<argc;j++)
							close(sfd[i-1]);
							int r=recv(nsfd,buf,1024,0);
							printf("requested service :%s\n",buf);
							strcpy(buf2,buf);
							char sno=buf[strlen(buf)-1];
							bzero(buf,1024);
							r=recv(nsfd,buf,1024,0);
							printf("client no is :%s\n",buf);
							char *logbuf=new char[10];
							strcpy(logbuf,"log");
							//printf("%s\n",logbuf);
							char buf3[1024];
							int ppid=fork();
							if(ppid==0)
							{
								dup2(nsfd,0);
								dup2(nsfd,1);
								execlp(buf2,buf2,buf,logbuf,(char *)NULL);
							}
							else
							{
								mkfifo("log",O_CREAT|0666);
								int logfd=open("log",O_RDONLY);
								//printf("%d\n",logfd);
								//printf("ppp\n");
								//bzero(buf,1024);
								while(read(logfd,buf3,1024)<0){}
								printf("server %c %s\n",sno,buf3);
							}
						}
						else
						{
							close(nsfd);
							/*bzero(buf,1024);
							int r=read(logfd,buf,1024);
							if(r<=0)
							printf("%d read fail\n",r);
							else
							printf("%s\n",buf);*/
						}
					}
				}
			}
		}
	}
}

