#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>

#define OUT(x...) fprintf(stderr,x)
#define IN(x...) fscanf(stdin,x)
#define swap(type,x,y) do{ type t=x; x=y; y=t; }while(0)

typedef struct{

        char Eng[30];
        char Kor[60];
        char total_kor[60];
        char tmp[5][60];

}Data;
Data W[120];

void get_eng(int);
void get_kor(int);
void detach(int size);


void random_num(int *,int );

int check_word(Data *list, int size);

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
                OUT("1.한글맞추기\n2.영어맞추기\n3.종료\n");
                IN("%d",&sel);

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
        int r=0;
        for(int i=0; i<size; i++)
        {
                strcpy(W[i].total_kor,W[i].Kor);

                p=strtok(W[i].Kor,",");

                if(p)   strcpy(W[i].tmp[r],p);

                while(p!=NULL) {

                        p=strtok(NULL,",");

                        if(p) { strcpy(W[i].tmp[++r],p); }

                }
                r=0;
        }
}

void random_num(int *list,int n)
{
        int j,k;

        srand((unsigned int)time(0));

        for(int i=0; i<n; i++) list[i]=i;
        for(int i=0; i<n; i++)
        {
                j=rand()%n;
                k=rand()%n;
                swap(int,list[j],list[k]);
        }
        return;
}

void get_eng(int count)
{
        Data get_word[count];
        int word_count=0;

#ifdef RANDOM
        int pos[count];
        random_num(pos,count);
#endif
#ifndef RANDOM
        int pos[count]; for(int i=0; i<count; i++) pos[i]=i;
#endif
        int r=0;
        char str[20];
        for(int i=0; i<count; i++)
        {
                OUT("%s -->",W[pos[i]].Eng);
                IN("%s",str);
                if((strcmp(str,"종료")==0) || (strcmp(str,"quit")==0)) break;
                if((strcmp(W[pos[i]].tmp[0],str) == 0) || (strcmp(W[pos[i]].tmp[1],str) == 0) ||
                   (strcmp(W[pos[i]].tmp[2],str) == 0) || (strcmp(W[pos[i]].tmp[3],str) == 0))
                {
                        OUT("맞았습니다\n");
                        word_count++;
                }else{
                        OUT("틀렸습니다\n");
                        OUT("-------------------\n");
                        OUT("%s, %s, %s\n",W[pos[i]].tmp[0],W[pos[i]].tmp[1],W[pos[i]].tmp[2]);
                        OUT("-------------------\n");
                        get_word[r++]=W[pos[i]];
                }
        }

        OUT("총 %d개 중 %d개 맞았습니다.\n틀린 개수 %d 개\n",count,word_count,count-word_count);
        check_word(get_word,r);
}

int check_word(Data *list,int size)
{
        FILE *fp=NULL;


        if((fp=fopen("incorrect.txt","wt"))==NULL)
        {
                perror("error");
                exit(1);
        }


        for(int i=0; i<size; i++)
                fprintf(fp,"%s %s\n",list[i].Eng,list[i].total_kor);

        fclose(fp);
        return 0;
}
void get_kor(int count)
{
        int word_count=0;
        int pos[count];
        random_num(pos,count);


        char str[20];
        for(int i=0; i<count; i++)
        {
                OUT("%s -->",W[i].total_kor);
                IN("%s",str);
                if((strcmp(str,"종료")==0) || (strcmp(str,"quit")==0)) break;
                if(strcmp(W[i].Eng,str)==0)
                {
                        OUT("맞았습니다\n");
                        word_count++;
                }else {
                        OUT("틀렸습니다\n");
                        OUT("-------------------\n");
                        OUT("%s \n",W[i].Eng);
                        OUT("-------------------\n");
                }
        }

        OUT("총 %d개 중 %d개 맞았습니다.\n틀린 개수 %d 개\n",count,word_count,count-word_count);
}
