#include<stdio.h>
#include<time.h>
#include<stdlib.h>
int compare(const void *a, const void *b)
{
                return *(int *)a - *(int *)b;
}

int main(){

                int arr[5][6];
                char c=',';
                int get_data;

                srand((unsigned)time(NULL));
                for(int i=0; i<5; i++)
                {
                                for(int j=0; j<6; j++)
                                {
                                        while(1)
                                        {
                                                arr[i][j]=rand()%45+1;
                                                get_data=0x00;
                                                for(int x=0; x<j; x++)
                                                {
                                                        if(arr[i][x]==arr[i][j])
                                                        {
                                                                get_data=0x01;
                                                                break;
                                                        }
                                                }if(!get_data) break;
                                        }
                                }
                }
                for(int i=0; i<5; i++)
                {
                        printf("%d ¼¼Æ® :",i+1);
                                for(int j=0; j<6; j++)
                                {
                                                printf("%d ",arr[i][j]);
                                }
                                printf("\n");
                }

                return 0;
}