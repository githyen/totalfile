#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/stat.h>
int main(int argc,char **argv)
{        
    if(argc != 3)
    {
        fprintf(stderr,"usage: %s <file name> <access mode>\n",argv[0]);
        fprintf(stderr,"access mode is octet number. must start %s\n",argv[0]);
        return 1;
    }
    if(access(argv[2],F_OK)!=0)
    {
        printf("%s is not existed\n",argv[1]);
        return 1;
    }
 
    mode_t mode = 0;
	if(argv[1][0]=='0')
	{
		sscanf(argv[1], "%o",&mode);
		if(chmod(argv[2],mode)!=0)
		    printf("fail change mode\n");
		else
			printf("success change mode\n");
	 }
	else if(argv[1][0] < '8')
	{
		sscanf(argv[1], "%o",&mode);

		if(chmod(argv[2],mode)< 0)
		    printf("fail change mode\n");
		else
			printf("success change mode\n");
	}
	else
	{
		fprintf(stderr,"chmod: Àß¸øµÈ ¸ðµå: `%s' \n",argv[1]);
		fprintf(stderr,"Try 'chmod --help' for more information.\n");
		exit(1);
	}
    return 0;    
}
