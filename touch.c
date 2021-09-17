#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<time.h>
#include<utime.h>
#include<sys/time.h>
#include<unistd.h>
#include<fcntl.h>
int main(int argc, char **argv)
{
	
	struct utimbuf ubuf;
	if(argc == 0)
	{
		fprintf(stderr,"touch: 파일 명령어 누락\nTry 'touch --help' for more information.\n");
		exit(1);
	}
	if(strncmp(argv[1],"--",2) == 0)
	{
		fprintf(stderr,"touch: 인식할 수 없는 옵션 '%s'\nTry 'touch --help' for more information.\n",argv[1]);
		exit(1);
	}
	for(int i=0; i<argc; i++)
	{
		if(access(argv[i],F_OK) < 0)
		{
			umask(0);
			creat(argv[i],0644);
		}
	}
	
	ubuf.actime = time((time_t *)0);
	ubuf.modtime = time((time_t *)0);

    utime(argv[1], NULL);
  //  utime(argv[1], &ubuf);
    exit(0);
}
