#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>


typedef struct{

	char Eng[20];
	char Kor[60];
	int file_count;
	char tmp[4][60];

}Data;
Data list[100];

void get_eng(int);
void get_kor(int);

int main(){

	FILE *fp=NULL;
	char str[100];

	if((fp=fopen("dic.txt","rt")) == NULL)
	{
		perror("open");
		exit(1);
	}
	int i=0;
	int r=1;
	char *p;
	while(fscanf(fp,"%d %s %s",&list[i].file_count,list[i].Eng,list[i].Kor)!=EOF)
	{
		p=strtok(list[i].Kor,",");
		if(p) {	strcpy(list[i].tmp[0],p); }
		while(p!=NULL) {
			p=strtok(NULL,",");
			if(p) {	strcpy(list[i].tmp[r],p); }
		}
		i++;
	}
	int sel;
	fclose(fp);
	while(1)
	{
		printf("1.kor\n2.eng\n");
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
		}else printf("틀렸습니다\n");
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
		printf("%s -->",list[i].Kor);
		scanf("%s",str);
		if(strcmp(str,"quit")==0) break;
		if(strcmp(list[i].Eng,str)==0)
		{
			printf("맞았습니다\n");
			word_count++;
		}else printf("틀렸습니다\n");
	}

	printf("총 %d개 중 %d개 맞았습니다.\n틀린 개수 %d 개\n",count,word_count,count-word_count);
}
