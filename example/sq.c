#include <stdio.h>
#define OUT(x...) fprintf(stderr,x)
#define IN(x...) fscanf(stdin,x)

int main()
{
   int num;
   IN("%d",&num);
   OUT("\n");
#if 1
    for (int i = 0; i < num; i++ )
    {
        for (int k = 0; k < i; k++ )    OUT(" ");
        for (int j = 0; j < num-i; j++ )  OUT("%d",i+1);
        OUT("\n");
    }

    for(int i=num; i>=1; i--)
    {
        for(int j=num; j>=i; j--) OUT("%d",j);
        OUT("\n");
    }
    

    OUT("\n");

   int i,j,k;
   int a=-2; 
   int sw=2;
   int n=num/2;
   
   for(i=0; i<num; i++)
   {
       a=a+sw; 
       
       if(a>=(n+2)) sw=-2;  
       if(num==3 && a==2) sw=-2; 
       for(j=0; j<a; j++) OUT(" "); 
       for(k=j; k<num; k++) OUT("%d",k+1); 
       OUT("\n");
   }

    OUT("\n");
    for (int i = 0; i < num; i++ )
    {
        for (int k = 0; k < i; k++ )    OUT(" ");
        for (int j = 1; j <=num-i; j++ ) {  OUT("%d",i+1); break; }
        OUT("\n");
    }
 
    OUT("\n");
    for (int i = 0; i < num; i++ )
    {
        for (int k = 0; k < i && i%2!=0; k++ )    OUT(" ");
        for (int j = 1; j <=num-i; j++ )  OUT("%d",i+1);
        OUT("\n");
    }
#endif 
    return 0;
}