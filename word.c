#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct node
{
        char eng[20];
        char kor[3][100];
		char _kor[40];
        struct node *link;
}NODE;
NODE *head=NULL;

void eng_read();
void kor_read();
void memory_destroy();
void kor_data();
void eng_data();

static float result=0;
static int flag_mode=0x00;
static char tmp[3][40];

void word_sort();
void get_token();

int main(){
    int sel;
    printf(" >> 영어 단어 맞추기 프로그램 << \n");

	while(1)
	{
		printf("1.한글 맞추기\n2.영어 맞추기\n");
		scanf("%d",&sel);

		if(sel==1) {  eng_read(); }
		if(sel==2) {  kor_read(); }
		if(sel==0) break;
	}
    memory_destroy();
    system("clear");
	exit(0);
}

void kor_read()
{
	FILE *fp=NULL;
	if((fp=fopen("dic.txt","rt"))==NULL)
	{
		perror("open");
		exit(1);
	}

	NODE *data;
	char eng[20],kor[100];

	char *p;
	int i=1;
	while(fscanf(fp,"%s %s",eng,kor)!=EOF)
	{
		NODE *cur=(NODE *)malloc(sizeof(NODE));
		cur->link=NULL;
		strcpy(cur->eng,eng);

		p=strtok(kor,",");
		if(p) { strcpy(cur->kor[0],p); }

		while(p!=NULL)
		{
			p=strtok(NULL,",");
			if(p) { strcpy(cur->kor[i],p); i++; }
		}

		if(head==NULL) head=cur; 
		else{
			for(data=head; data->link!=NULL; data=data->link);
			data->link=cur;
		}
	}

	kor_data();
	fclose(fp);

}

void eng_read()
{
	FILE *fp=NULL;
    fp=fopen("dic.txt","rt");
    if(fp==NULL)
    {
        fprintf(stderr,"FILE OPEN ERROR \n");
        exit(1);
    }
    NODE *prev=NULL;
    char eng[20];
    char kor[100];

        while(fscanf(fp,"%s %s",eng,kor)!=EOF)
    	{
            NODE *cur=(NODE *)malloc(sizeof(NODE));
            cur->link=NULL;
            strcpy(cur->eng,eng);
			strcpy(cur->_kor,kor);

            if(head==NULL)
            {
           	    head=cur;
            }
            else
            {
           		for(prev=head; prev->link!=NULL; prev=prev->link);
           		prev->link=cur;
        	}
   		}

	eng_data();
	fclose(fp);
}

void memory_destroy()
{
    NODE *cur=NULL;
    NODE *tmp=NULL;
    cur=head;
    while(cur!=NULL)
    {
        tmp=cur->link;
        free(cur);
        cur=tmp;
    }
    head=NULL;
}


void kor_data()
{
	NODE *cur=NULL;
#if 1
	char kor[40];
	for(cur=head; cur!=NULL; cur=cur->link)
	{
		printf("%s --->",cur->eng);
		scanf("%s",kor);
		if(strcmp(cur->kor[0],kor)==0 || strcmp(cur->kor[1],kor)==0 || strcmp(cur->kor[2],kor)==0)
		{
			printf("correct!\n");
			result+=25.0;
		}
		else
			printf("incorrect!\n");
	}
	printf("result : %.2f\n",result);
#endif

}
void eng_data()
{
        NODE *cur=NULL;
        char in[100];
        for(cur=head; cur!=NULL; cur=cur->link)
        {
                printf("%s --->",cur->_kor);
                scanf("%s",in);
                if(strcmp(in,"quit")==0) break;
                if(strcmp(cur->eng,in)==0)
                {
                        printf("correct!\n");
                        result+=25.0;
                }
                else
                        printf("incorrect!\n");
        }
        printf("word result : %.2f\n",result);
		printf("프로그램을 종료합니다.\n");
}




