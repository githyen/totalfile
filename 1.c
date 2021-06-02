#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

void set_data(int n, int m,int set_mode)
{
	pid_t pid;
	int y;

		for(int ch=0; ch<m; ch++)
		{
			if((pid=vfork()) < 0) { perror("error"); exit(1); }
			else if(pid==0)
			{
				if(ch==0)
				{
					y=n/m+set_mode;
					for(int i=1; i<y; i++)
						printf("0 i value : %d \n",i);
					printf("process ID : %d \n",getpid());
					exit(0);
				}
				if(ch==1)
				{
					for(int i=y; i<y+(n/m); i++)
						printf("1 i value : %d \n",i);
					printf("process ID : %d \n",getpid());
					y=y+(n/m);
					exit(0);
				}

				if(ch<m)
				{
					for(int i=y; i<y+(n/m); i++)
						printf(" 2 i value : %d \n",i);
					printf("process ID : %d \n",getpid());
					y=y+(n/m);
					exit(0);
				}

				if(ch==(m-1))
				{
					for(int i=y; i<n; i++)
						printf("3 i value : %d \n",i);
					printf("process ID : %d \n",getpid());
				}
			}
			else waitpid(pid,NULL,0);
		}
}

int main(){

	int n,m;
	pid_t pid;

	scanf("%d %d",&n,&m);

#if 1
		if(n%m==0)
		{
			set_data(n,m,n%m);
			exit(0);
		}
		else if(n%m==1)
			set_data(n,m,n%m);
		else if(n%m==2)
			set_data(n,m,n%m);
		else if(n%m==3)
			set_data(n,m,n%m);
		else if(n%m==4)
			set_data(n,m,n%m);
		else if(n%m==5)
			set_data(n,m,n%m);
#endif
	return 0;
}
