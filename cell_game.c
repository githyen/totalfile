#include<stdio.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/time.h>
#define size 10000

#define LIVE 1
#define DEATH 0


typedef struct{
		int r;
		int w;
		
		int **matrix;
		int **tmp;
}Graph;


Graph *init(int,int);
void free_g(Graph *);

static Graph *g=NULL;
static FILE *fp=NULL;
int file_count=1;
static int avg[20]; 
int array[size][size];

int tmp[size][size]={0,};

int flag_mode=0x00;

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
void get_menu()
{
	printf("1.PROGRAM END\n2.Sequential processing\n3.Process parallel\n4.Thread parallel\n");
}

int get_value(Graph *g,int r, int c);
int get_neighbor(Graph *g,int r, int c);

void get_data(char *fname);
void get_rvalue(int,int );
void get_matrix(int **,int,int,int);

void *set_thread(void *arg);
void set_Edge(Graph *);
void set_process(int child,int get_mode);

void serial_processing(Graph *g,int n);
void parallel_process(Graph *g,int child,int n);
void parallel_thread(int child, int n);



void get_rvalue(int row, int child)
{
	int i=1;

	while(row--)
	{
		avg[i]+=1;
		if(i==child) i=1;
		else i++;
	}

	for(int i=1; i<=child; i++)
		avg[i]+=avg[i-1];

	avg[0]=1;
	avg[child]=avg[child]-1;
}
		
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
	int count=0;

	int mode=0;
	for(int ch=0; ch<child; ch++)
	{
		if((pid=vfork()) < 0) { perror("error"); exit(1); }
		else if(pid==0)
		{
				for(int i=avg[mode]; i<avg[mode+1]; i++)
				{
					for(int j=1; j<g->w-1; j++)
					{
						count=get_neighbor(g,i,j);
						get_matrix(g->matrix,i,j,count);
					}
				}
				_exit(0);
		}
		else{
			waitpid(pid,NULL,0);
			printf("Process ID : %d \n",pid);
			mode++;
			
		}
	}
}



void print_data(Graph *g)
{
	for(int i=0; i<g->r; i++)
	{
		for(int j=0; j<g->w; j++)
			array[i][j]=g->matrix[i][j];
	}
}
void push_data(Graph *g,int n)
{
	set_Edge(g);
	FILE *fp=NULL;
	char fname[14];
	sprintf(fname,"gen_%d.matrix",file_count++);
	
	if(n==1)
	{
		memset(fname,0x00,strlen(fname));
		memcpy(fname,"output.matrix",14);
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
		fprintf(stderr,"file open error");
		exit(1);
	}
	int c,n;
	int child;

	struct timeval start,end;
	double d_time;

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
			file_count=1;
			gettimeofday(&end,NULL);
			d_time=(end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1e6);
			printf(" Sequential processing run time : %f seconds\n",d_time);
		}
		if(c==3) {

			get_data(argv[1]);
			printf("what generation would you like to proceed with??");
			scanf("%d",&n);

			printf("how many process do you want to create?"); 
			scanf("%d",&child);
			gettimeofday(&start,NULL);
			get_rvalue(g->r,child);
			parallel_process(g,child,n);
			gettimeofday(&end,NULL);
			d_time=(end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1e6);
			free_g(g);
			file_count=1;
			printf("process parallel run time : %f seconds\n",d_time);

		}
		if(c==4)
		{
			flag_mode=0x01;
			gettimeofday(&start,NULL);
			get_data(argv[1]);
			printf("what generation would you like to proceed with??");
			scanf("%d",&n);

			printf("how many thread do you want to create?"); 
			scanf("%d",&child);
			get_rvalue(g->r,child);
			print_data(g);
			parallel_thread(child,n);
			gettimeofday(&end,NULL);
			d_time=(end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1e6);

			free_g(g);
			file_count=1;
			printf("thread parallel run time : %f seconds\n",d_time-n);

		}
	}
   	exit(0);
}

void parallel_process(Graph *g,int child,int n)
{
		if(n==0) return;

		set_process(child,0);

		for(int i=0; i<g->r; i++)
		{
			for(int j=0; j<g->w; j++)
				g->matrix[i][j]=g->tmp[i][j];
		}
		push_data(g,n);
		parallel_process(g,child,n-1);
}
void parallel_thread(int child,int n)
{
	if(n==0) return;
	pthread_t tid[child];
	int idx[child];
	for(int i=0; i<child; i++)
	{
		idx[i]=i;
		if(pthread_create(&tid[i],NULL,set_thread,(void *)&idx[i])!=0)
		{
			fprintf(stderr,"thread create failed\n");
			exit(EXIT_FAILURE);
		}
	}
	sleep(1);
	pthread_cond_broadcast(&cond);


	for(int i=0; i<child; i++)
	{
		if(pthread_join(tid[i],NULL))
		{
			fprintf(stderr,"thread join failed\n");
			exit(EXIT_FAILURE);
		}
	}
	if(pthread_mutex_destroy(&mutex))
	{
		fprintf(stderr,"mutex destroy error \n");
		exit(EXIT_FAILURE);
	}

	if(pthread_cond_destroy(&cond))
	{
		fprintf(stderr,"cond destroy error \n");
		exit(EXIT_FAILURE);
	}

	for(int i=0; i<g->r; i++)
	{
		for(int j=0; j<g->w; j++)
		{
			array[i][j]=tmp[i][j];
			g->matrix[i][j]=tmp[i][j];
		}
	}
	push_data(g,n);
	parallel_thread(child,n-1);

}
void *set_thread(void *arg)
{
	printf("thread ID : %ld \n",pthread_self());
	int idx=*((int *)arg);
	int count=0;

	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&cond,&mutex);
	for(int i=avg[idx]; i<avg[idx+1]; i++)
	{
		for(int j=1; j<g->w-1; j++)
		{
			count=get_neighbor(NULL,i,j);
			if(array[i][j])
			{
				if(count <=2 || count >=7)
					tmp[i][j]=DEATH;
				else if(count >=3 && count <=6)
					tmp[i][j]=LIVE;
			}else{
				if(count == 4) tmp[i][j]=LIVE;
				else		   tmp[i][j]=DEATH;
			}
		}
	}
	pthread_mutex_unlock(&mutex);
	return NULL;
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
	if(flag_mode==0)
	{
	
		if(g->matrix[r][c] == LIVE)
			return 1;
		else
			return 0;
	}
	else
	{
		if(array[r][c] == LIVE)
			return 1;
		else
			return 0;
	}
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
			get_matrix(g->matrix,i,j,count);
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

