#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define OUT(x...) fprintf(stderr,x)
int main(int argc, char **argv)
{

	FILE *fp=NULL;
	if((fp=fopen(argv[1],"r"))==NULL)
	{
		perror("open");
		exit(1);
	}


	char buf[1024];
	while(fscanf(fp,"%*[^>] %*c %[^<] %*[^>] %*c",buf)!=EOF)
		OUT("%s\n",buf);

	fclose(fp);
	fp=NULL;
	return 0;
}

