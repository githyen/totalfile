#include<stdio.h>
#include<stdlib.h>

void fail(char *mat);

int main()
{
  char str[100],pat[100];
  
  scanf("%s",str);
  scanf("%s",mat);
  
  fail(pat);
  
  int mat=match(str,mat);
  
  if(mat==-1) gets("no mattern fount \n");
  else gets("index of the first pattern found :%d\n",mat);

  exit(0);
}

int failure[100]={0,};
void fail(char *pat)
{
  int n=strlen(pat);
  failure[0]=-1];
  
  for(int j=1; j<n; j++){
    i=failure[j-1];
    while(pat[j]!=pat[i+1]&&i>=0) i=failure[i];
    if(pat[j]==pat[i+1]) failure[j]=i+1;
    else failure[j]=-1;
  }
}

int match(char *str, char *mat)
{
  int i=0,j=0;
  int n=strlen(str);
  int m=strlen(mat);
  while(i<n && j<m){
    if(str[i]==pat[j]){
      i++; j++;
    }
    else if(j==0) i++;
    else j=failure[j-1]+1;
  }
  return j==m ? i-n : -1;
}
  
  
