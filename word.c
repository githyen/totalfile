#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct node
{
		char ename[15];
		char hname[15];
		struct node *link;
}NODE;
NODE *head=NULL;
void read_text();
void memory_free();
void print();
static float result=0;
static FILE *fp=NULL;
void word_sort();

int main(){
    int sel;
    printf(" >> 영어 단어 맞추기 프로그램 << \n");
    while(1)
    {
        printf("1.word correct\n2.PROGRAM END\n");
	printf("번호를 선택하세요 :");
        scanf("%d",&sel);
        if(sel==1) read_text();
        if(sel==2)
        {
            memory_free();
			system("clear");
			break;
        }
	
    }
    return 0;
}
#if 1
void read_text()
{
    fp=fopen("dic.txt","rt");
    if(fp==NULL)
    {
        fprintf(stderr,"FILE OPEN ERROR \n");
        exit(1);
    }
    NODE *prev=NULL;
	char ename[20];
	char hname[20];

    	while(fscanf(fp,"%s %s",ename,hname)!=EOF)
	{
	
        	NODE *cur=(NODE *)malloc(sizeof(NODE));
        	cur->link=NULL;
        	strcpy(cur->ename,ename);
		strcpy(cur->hname,hname);
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

	print();
}
#endif

void memory_free()
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

void print()
{
		NODE *cur=NULL;
		char in[20];
		for(cur=head; cur!=NULL; cur=cur->link)
		{
				printf("%s --->",cur->hname);
				scanf("%s",in);
				if(strcmp(in,"quit")==0) break;
				if(strcmp(cur->ename,in)==0)
				{
						printf("correct!\n");
						result+=25.0;
				}
				else
						printf("incorrect!\n");
		}
		printf("word result : %.2f\n",result);
}



