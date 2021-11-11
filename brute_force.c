#include<stdio.h>
#include<string.h>

void bf_match(char *ar, char *cr);

static int arr[10];
static int count=0;

int main(){

        char str1[128];
        char str2[20];
        scanf("%[^\n]",str1);
        getchar();
        scanf("%[^\n]",str2);

        bf_match(str1,str2);

        if(count==0)
                printf("-1\n");

        else{
                for(int i=0; i<count; i++)
                        printf("%d ",arr[i]);
                }

        return 0;
}


void bf_match(char *ar, char *cr)
{

        int pt=0;
        int pr=0;
        int len=strlen(cr);
        while(ar[pt]!='\0')
        {
                if(ar[pt]==cr[pr])
                {
                        pt++;
                        pr++;
                        if(pr == len)
                        {
                                pr=0;
                                arr[count++]=pt-len;
                        }
                }
                else
                {
                        pt=pt-pr+1;
                        pr=0;
                }

        }
        return;
}
