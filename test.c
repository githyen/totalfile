#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include"matrix.h"

#define LIVE 1
#define DEATH 0


static Graph *g=NULL;
static FILE *fp=NULL;
static int r=0,w=0;

void get_menu()
{
	printf("1.Program End\n2.Serial Processing\n3.Process Parallel Processing\n4.Thread Parallel Processing\n");
}

int get_value(Graph *g,int r, int c);
int get_neighbor(Graph *g,int r, int c);
void get_data(char *fname);

void serial_processing(Graph *g,int n);
void parallel_processing(Graph *g, int n);
void parallel_thread(Graph *g, int n);

void push_data(Graph *g,int n)
{


	static char i='1';
	FILE *fp=NULL;
	static char fname[]="gen_0.matrix";

	fname[4]=i;

	if((fp=fopen(fname,"wt"))==NULL)
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
	i++;
}

void get_data(char *fname)
{
	char character;

    fp=fopen(fname,"rt");
    
	int fd=fileno(fp);
	int n;
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
	printf("몇세대 진행 하시겠습니까?");
	scanf("%d",&n);
	serial_processing(g,n);
	return;
	

}

int main(int argc, char **argv){
	
	if(argc == 1)
	{
		perror("error");
		exit(1);
	}
	int c;
	int child;
	while(1)
	{
		get_menu();
		scanf("%d",&c);

		if(c==1) { break; }
		if(c==2) {	get_data(argv[1]); free_g(g); }
		if(c==3) {
			printf("몇개의 프로세스를 생성하시겠습니까?"); 
			scanf("%d",&child);
			putchar('\n');
		}
		if(c==4)
		{
			
			printf("몇개의 쓰레드를 생성하시겠습니까?"); 
			scanf("%d",&child);
			putchar('\n');
		}
	}
//	free_g(g);
   	exit(0);
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

	for(int i=0; i<g->r; i++)
		memset(g->tmp[i],0,sizeof(int)*g->r);

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
#if 1
	for(int i=0; i<g->r; i++)
	{
		for(int j=0; j<g->w; j++)
			g->matrix[i][j]=g->tmp[i][j];
	}
#endif
	push_data(g,n);
	serial_processing(g,n-1);

}

Graph *init(int r, int w)
{
		Graph *new=(Graph *)malloc(sizeof(Graph));
		
		new->r=r;
		new->w=w;
		
		new->matrix=(int **)calloc(new->r,sizeof(int *));
		new->tmp=(int **)calloc(new->r,sizeof(int *));

		for(int i=0; i<new->r; i++)
		{
				new->matrix[i] = (int *)calloc(new->w,sizeof(int));
				new->tmp[i] = (int *)calloc(new->w,sizeof(int));
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

