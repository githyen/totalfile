#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>

int main(){

	int n,m;
	scanf("%d %d",&n,&m);
	
	int arr[n][m];

	FILE *fp=NULL;

	if((fp=fopen("input.matrix","wt")) == NULL)
	{
		perror("error");
		exit(1);
	}
	int fd=fileno(fp);

	srand((unsigned)time(NULL));
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<m; j++)
			arr[i][j]=rand()%+2;
	}
	dup2(fd,1);
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<m; j++)
			printf("%d ",arr[i][j]);
		printf("\n");
	}
	
	exit(0);
}


