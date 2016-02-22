#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<stdlib.h>
typedef struct sockaddr_in sckadr;
int main(int argc,char **argv)
{
	int port=atoi(argv[1]);
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	printf("socket failed\n");
	sckadr seradr;
	seradr.sin_family=AF_INET;
	seradr.sin_port=htons(port);
	seradr.sin_addr.s_addr=htonl(INADDR_ANY);
	bzero(seradr.sin_zero,8);
	int n=connect(sfd,(struct sockaddr *)&seradr,sizeof(seradr));
	if(n<0)
	printf("connect failed\n");
	char *buf=new char[1024];
	printf("enter service :");
	scanf("%s",buf);
	int s=send(sfd,buf,strlen(buf),0);
	bzero(buf,1024);
	printf("enter client no :");
	scanf("%s",buf);
	s=send(sfd,buf,strlen(buf),0);
	while(1)
	{
		scanf("%s",buf);
		int m=send(sfd,buf,strlen(buf),0);
		int p=recv(sfd,buf,1024,0);
		printf("%s\n",buf);
	}
}
