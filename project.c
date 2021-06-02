#include<stdio.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include"matrix.h"

#define LIVE 1
#define DEATH 0

static Graph *g=NULL;
static FILE *fp=NULL;
static char i='1';

void get_menu()
{
	printf("1.프로그램 종료.\n2.순차처리.\n3.프로세스 병렬처리.\n4.쓰레드 병렬처리.\n");
}

int get_value(Graph *g,int r, int c);
int get_neighbor(Graph *g,int r, int c);
void get_data(char *fname);

void serial_processing(Graph *g,int n);
void parallel_processing(Graph *g,int child,int n);
void parallel_thread(int child, int n);


void get_matrix(int **ar, int i, int j,int count)
{
	if(ar[i][j])
	{
		if(count <=2 || count >= 7)
			g->tmp[i][j]=DEATH;
		else if(count >=3 && count <= 6)
			g->tmp[i][j]=LIVE;
	}else{
		if(count == 4) g->tmp[i][j]=LIVE;
		else		   g->tmp[i][j]=DEATH;
	}
}

void set_process(int child,int get_mode)
{
	pid_t pid;
	int y;
	int count=0;

	int stat;
	for(int ch=0; ch<child; ch++)
	{
		if((pid=vfork()) < 0) { perror("error"); exit(1); }
		else if(pid==0)
		{
			if(ch==0)
			{
				y=g->r/child+get_mode;
				for(int i=1; i<y; i++)
				{
					for(int j=1; j<g->w-1; j++)
					{
						count=get_neighbor(g,i,j);
						get_matrix(g->matrix,i,j,count);
					}
				}
				printf("process ID : %d \n",getpid());
				exit(0);
			}
			if(ch==1)
			{
				for(int i=y; i<y+(g->r/child); i++)
				{
					for(int j=1; j<g->w-1; j++)
					{
						count=get_neighbor(g,i,j);
						get_matrix(g->matrix,i,j,count);
					}
				}
				printf("process ID : %d \n",getpid());
				y+=g->r/child;
				exit(0);
			}

			if(ch<child)
			{
				for(int i=y; i<y+(g->r/child); i++)
				{
					for(int j=1; j<g->w-1; j++)
					{
						count=get_neighbor(g,i,j);
						get_matrix(g->matrix,i,j,count);
					}
				}
				printf("process ID : %d \n",getpid());
				y+=g->r/child;
				exit(0);
			}
			exit(0);
		}
		else
		{
			waitpid(pid,&stat,0);
	//		printf("parent process status : %d \n",stat);
		}
	}
}



void print_data(Graph *g)
{
	for(int i=0; i<g->r; i++)
	{
		for(int j=0; j<g->w; j++)
			printf("%d ",g->matrix[i][j]);
		printf("\n");
	}
}
void push_data(Graph *g,int n)
{
#if 1
	set_Edge(g);
	FILE *fp=NULL;
	char fname[]="gen_1.matrix";
	char out[]="output.matrix";
	
	fname[4]=i;
	if(n==1)
	{
		memset(fname,0x00,strlen(fname));
		memcpy(fname,out,strlen(out));
	}

	if((fp=fopen(fname,"w"))==NULL)
	{
		perror("error");
		exit(1);
	}

	for(int i=0; i<g->r; i++)
	{
		for(int j=0; j<g->w; j++)
			fprintf(fp,"%d ",g->matrix[i][j]);
		fprintf(fp,"\n");
	}
	fclose(fp);
	fp=NULL;
	i++;
#endif
}

void get_data(char *fname)
{
	char character;

    int r=0,w=0;
    fp=fopen(fname,"rt");
    
	int fd=fileno(fp);
    if ((fd = open(fname,O_RDONLY)) < 0) {
            fprintf(stderr,"open error \n");
			exit(1);
    }


	while (1) {
	    if (read(fd, &character, 1) > 0)
        {
		    w++;
            if (character == '\n') r++;
        }
        else   break;

    }
    close(fd);
	g=init(r,w/(r*2));
    
	for(int i=0; i<g->r; i++)
    {
          for(int j=0; j<g->w; j++)
	           fscanf(fp,"%d",g->matrix[i]+j);
    }
	
	fclose(fp);
	fp = NULL;
	return;
	

}

int main(int argc, char **argv){
	
	if(argc == 1)
	{
		fprintf(stderr,"잘못된 접근 : 파일이름을 입력하세요.");
		exit(1);
	}
	int c,n;
	int child;

	struct timeval start,end;
	double delay_time;
	while(1)
	{
		get_menu();
		scanf("%d",&c);

		if(c==1) { break; }
		if(c==2) {	

			gettimeofday(&start,NULL);
			get_data(argv[1]); 
			printf("what generation would you like to proceed with??");
			scanf("%d",&n);
			if(n >= 1)
				serial_processing(g,n);
			free_g(g); 
			i='1';
			gettimeofday(&end,NULL);
			delay_time=(end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000000);
			printf("%f seconds\n",delay_time);

		}
		if(c==3) {

			gettimeofday(&start,NULL);
			get_data(argv[1]);

			
			printf("what generation would you like to proceed with??");
			scanf("%d",&n);

			printf("how many process do you want to create?"); 
			scanf("%d",&child);
			
			parallel_processing(g,child,n);
			free_g(g);
			i='1';
			gettimeofday(&end,NULL);
			delay_time=(end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000000);
			printf("%f seconds\n",delay_time);

		}
		if(c==4)
		{
			get_data(argv[1]);
			printf("what generation would you like to proceed with??");
			scanf("%d",&n);

			printf("how many process do you want to create?"); 
			scanf("%d",&child);
			parallel_thread(child,n);
			free_g(g);
			i='1';

		}
	}
   	exit(0);
}

void parallel_processing(Graph *g,int child,int n)
{
		if(n==0) return;
		for(int i=0; i<10; i++)
		{
			if(g->r%child==i)
				set_process(child,g->r%child);
		}

		for(int i=0; i<g->r; i++)
		{
			for(int j=0; j<g->w; j++)
				g->matrix[i][j]=g->tmp[i][j];
		}
		push_data(g,n);
//		print_data(g);
//		putchar('\n');
		parallel_processing(g,child,n-1);
}
void parallel_thread(int child,int n)
{

	pthread_t tid;
	pthread_attr_t attr;
	int rc;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	
	for(int i=0; i<child; i++)
	{
		rc=pthread_create(&tid,&attr,set_thread,NULL);
		
		if(rc)
		{
			fprintf(stderr,"ERROR : return code from pthread_create() is %d \n",rc); 
			exit(1);
		}

		rc=pthread_join(tid,NULL);

		if(rc)
		{
			fprintf(stderr,"return code from pthread_join() is  %d \n",rc);
		}
	
	}
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);


}

void set_Edge(Graph *g)
{
	for(int i=0; i<g->r; i++)
		g->matrix[i][0]=g->matrix[i][g->w-1]=0;
	for(int j=0; j<g->w; j++)
		g->matrix[0][j]=g->matrix[g->r-1][j]=0;
}


int get_value(Graph *g,int r, int c)
{
	if(g->matrix[r][c] == LIVE)
		return 1;
	else
		return 0;
}

int get_neighbor(Graph *g,int r,int c)
{
	int count=0;
	
	count+=get_value(g,r,c+1);
	count+=get_value(g,r,c-1);

	count+=get_value(g,r-1,c-1);
	count+=get_value(g,r-1,c);
	count+=get_value(g,r-1,c+1);

	count+=get_value(g,r+1,c-1);
	count+=get_value(g,r+1,c);
	count+=get_value(g,r+1,c+1);

	return count;

}


void serial_processing(Graph *g,int n){
	

	if(n==0) return;
	int count=0;

	for(int i=1; i<g->r-1; i++)
	{
		for(int j=1; j<g->w-1; j++)
		{
			count=get_neighbor(g,i,j);
			if(g->matrix[i][j])
			{
				if(count <= 2 || count >= 7)
					g->tmp[i][j]=DEATH;
				else if(count >= 3 && count <= 6)
					g->tmp[i][j]=LIVE;
			}else{
				if(count == 4)
					g->tmp[i][j]=LIVE;
				else
					g->tmp[i][j]=DEATH;
			}
		}
	}
	for(int i=0; i<g->r; i++)
	{
		for(int j=0; j<g->w; j++)
			g->matrix[i][j]=g->tmp[i][j];
	}

	push_data(g,n);
	serial_processing(g,n-1);

}

Graph *init(int r, int w)
{
		Graph *new=(Graph *)malloc(sizeof(Graph));
		
		new->r=r;
		new->w=w;
		
		new->matrix=(int **)calloc(new->r , sizeof(int *));
		new->tmp=(int **)calloc(new->r , sizeof(int *));

		for(int i=0; i<new->r; i++)
		{
				new->matrix[i] = (int *)calloc(new->w , sizeof(int));
				new->tmp[i] = (int *)calloc(new->w , sizeof(int));
		}
		return new;
}

void free_g(Graph *G)
{
		for(int i=0; i<G->r; i++)
		{
				free(G->matrix[i]);
				free(G->tmp[i]);
		}

		free(G->matrix);
		free(G->tmp);
		G->matrix=NULL;
		G->tmp=NULL;
		free(G);
}

