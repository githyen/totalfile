#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc, char **argv)
{

	FILE *fp=NULL;
	int fd;


	if((fp=fopen(argv[1],"rt"))==NULL)
	{
		perror("open");
		exit(1);
	}
	fd=open(argv[2],O_WRONLY|O_CREAT,0644);

	char eng[20];
	char kor[40];
	int count=1;
	dup2(fd,1);
	while(fscanf(fp,"%s %s",eng,kor)!=EOF)
	{
		printf("%d %s %s\n",count,eng,kor);
		count++;
	}
	fclose(fp);
	close(fd);
	exit(0);
}
