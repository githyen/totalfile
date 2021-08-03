#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>


typedef struct{

	char Eng[20];
	char Kor[100];
	char total_kor[100];
	int file_count;
	char tmp[4][100];

}Data;
Data list[100];

void get_eng(int);
void get_kor(int);

int main(int argc, char **argv){

	FILE *fp=NULL;
	char str[100];

	if((fp=fopen(argv[1],"rt")) == NULL)
	{
		perror("open");
		exit(1);
	}
	int i=0;
	int r=1;
	char *p;

#if 1
	while(fscanf(fp,"%d %s %s",&list[i].file_count,list[i].Eng,list[i].Kor)!=EOF)
	{
		strcpy(list[i].total_kor,list[i].Kor);
		p=strtok(list[i].Kor,",");
		if(p) {	strcpy(list[i].tmp[0],p); }
		while(p!=NULL) {
			p=strtok(NULL,",");
			if(p) {	strcpy(list[i].tmp[r],p);  }
		}
		i++;
	}
#endif
	int sel;
	fclose(fp);
	fp=NULL;
	while(1)
	{
		printf("1.한글맞추기\n2.영어맞추기\n");
		scanf("%d",&sel);

		if(sel==1) get_kor(i);
		if(sel==2) get_eng(i);
		if(sel==0) break;
	}
	exit(0);
}

void get_eng(int count)
{
	int word_count=0;

	int i=1;
	char str[20];
#if 1
	for(int i=0; i<count; i++)
	{
		printf("%s -->",list[i].Eng);
		scanf("%s",str);
		if(strcmp(str,"종료")==0) break;
		if((strcmp(list[i].tmp[0],str) == 0) || (strcmp(list[i].tmp[1],str) == 0) ||
		   (strcmp(list[i].tmp[2],str) == 0) || (strcmp(list[i].tmp[3],str) == 0))
		{
			printf("맞았습니다\n");
			word_count++;
		}else{
			printf("틀렸습니다\n");
			printf("-------------------\n");
			printf("%s, %s, %s\n",list[i].tmp[0],list[i].tmp[1],list[i].tmp[2]);
			printf("-------------------\n");
		}
	}

	printf("총 %d개 중 %d개 맞았습니다.\n틀린 개수 %d 개\n",count,word_count,count-word_count);
#endif
}

void get_kor(int count)
{
	int word_count=0;
	char str[20];
	for(int i=0; i<count; i++)
	{
		printf("%s -->",list[i].total_kor);
		scanf("%s",str);
		if(strcmp(str,"quit")==0) break;
		if(strcmp(list[i].Eng,str)==0)
		{
			printf("맞았습니다\n");
			word_count++;
		}else {
			printf("틀렸습니다\n");
			printf("-------------------\n");
			printf("%s \n",list[i].Eng);
			printf("-------------------\n");
		}
	}

	printf("총 %d개 중 %d개 맞았습니다.\n틀린 개수 %d 개\n",count,word_count,count-word_count);
}
