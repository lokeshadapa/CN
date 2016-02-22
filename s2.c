#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
int main(int argc,char **argv)
{
//	for(int i=1;i<argc;i++)
//	write(1,argv[i],strlen(argv[i]));
	mkfifo(argv[2],O_CREAT|0666);
	int logfd=open(argv[2],O_RDWR);
	if(logfd<0)
	printf("log failed\n");
	char *logbuf=new char[1024];
	strcpy(logbuf,"servicing client");
	strcat(logbuf,argv[1]);
	strcat(logbuf,"\n");
	int w=write(logfd,logbuf,1024);
	if(w<0)
	printf("log failed\n");
//	write(1,logbuf,1024);
	char *buf=new char[1024];
	//scanf("%s",buf);
	while(1)
	{
		int n=read(0,buf,1024);
		for(int i=0;i<n;i++)
		{
			buf[i]=tolower(buf[i]);
		}
		write(1,buf,1024);
	}
}
