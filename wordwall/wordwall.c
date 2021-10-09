#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

typedef struct{

	char Eng[20];
	char Kor[50];
	char total_kor[50];
	char tmp[5][50];

}Data;
Data W[120];


static int flag=0x00;
pthread_t tid;

void get_eng(int);
void get_kor(int);
void detach(int size);

void *set_time(void *);

static int timer=5;

int compare(const void *a, const void *b)
{
	return (strcmp((char *)a,(char *)b));
}
int main(int argc, char **argv){

	FILE *fp=NULL;

	if((fp=fopen(argv[1],"rt")) == NULL)
	{
		perror("open");
		exit(1);
	}
	int i=0;

	while(fscanf(fp,"%s %s",W[i].Eng,W[i].Kor)!=EOF)
				i++;
	int sel;
	fclose(fp);
	rewind(stdout);
	fp=NULL;
	detach(i);
	while(1)
	{
		printf("1.한글맞추기\n2.영어맞추기\n3.종료\n");
		scanf("%d",&sel);

		if(sel==1) get_kor(i); 
		if(sel==2) get_eng(i);
		if(sel==3) break;
	}
	exit(0);
}

void detach(int size)
{
#ifdef SORT
	qsort(W,size,sizeof(W[0]),compare);
#endif
	char *p=NULL;
	int r=1;
	for(int i=0; i<size; i++)
	{
		strcpy(W[i].total_kor,W[i].Kor);

		p=strtok(W[i].Kor,",");

		if(p) 	strcpy(W[i].tmp[0],p); 
		
		while(p!=NULL) {

			p=strtok(NULL,",");

			if(p) {	strcpy(W[i].tmp[r],p);r++; }
		
		}
		r=1;
	}
}

void get_eng(int count)
{
	int word_count=0;


	char str[20];
#if 1
	for(int i=0; i<count; i++)
	{
		printf("%s -->",W[i].Eng);
		if(pthread_create(&tid,NULL,set_time,(void *)&timer)!=0)
		{
			perror("create");
			exit(1);
		}
		if(scanf("%s",str))
			flag=0x01;

		if((strcmp(str,"종료")==0) || (strcmp(str,"quit")==0)) break;
		if((strcmp(W[i].tmp[0],str) == 0) || (strcmp(W[i].tmp[1],str) == 0) ||
		   (strcmp(W[i].tmp[2],str) == 0) || (strcmp(W[i].tmp[3],str) == 0))
		{
			printf("맞았습니다\n");
			word_count++;
		}else{
			printf("틀렸습니다\n");
			printf("-------------------\n");
			printf("%s, %s, %s\n",W[i].tmp[0],W[i].tmp[1],W[i].tmp[2]);
			printf("-------------------\n");
		}
		pthread_join(tid,NULL);
		flag=0x00;
	}

	printf("총 %d개 중 %d개 맞았습니다.\n틀린 개수 %d 개\n",count,word_count,count-word_count);
#endif
	return;
}

void *set_time(void *t)
{
	int count_time=*((int *)t);
	while(count_time)
	{
		if(flag==0x01) break;
		printf("\n%70d sec\n",count_time--);
		sleep(1);
		if(count_time==0) { printf("틀렸습니다.\n");  return NULL; }
	}
	return NULL;
}
void get_kor(int count)
{
	int word_count=0;
	char str[20];
	for(int i=0; i<count; i++)
	{
		printf("%s -->",W[i].total_kor);	

		scanf("%s",str);

		if((strcmp(str,"종료")==0) || (strcmp(str,"quit")==0)) break;
		if(strcmp(W[i].Eng,str)==0)
		{
			printf("맞았습니다\n");
			word_count++;
		}else {
			printf("틀렸습니다\n");
			printf("-------------------\n");
			printf("%s \n",W[i].Eng);
			printf("-------------------\n");
		}
	}
	printf("총 %d개 중 %d개 맞았습니다.\n틀린 개수 %d 개\n",count,word_count,count-word_count);
}
