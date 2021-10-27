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

int inaccurate_word(Data *list, int size);

int compare(const void *a, const void *b)
{
	return (strcmp((char *)a,(char *)b));
}

