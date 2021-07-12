#include"head.h"
#define PATH_LENGTH 200
void printstat(char *fname, struct stat *st);
void seek_dir(char *dir, int opt);
void color_change(struct stat *st, char *name);
static int flag_mode = 0x00, count = 0;
static char *name[200];
static int total = 0;
typedef struct{
	char name[20];
	time_t time;
}ls_t;
ls_t array[200];

int compare(const void *a, const void *b)
{	
	ls_t *sa = (ls_t *)a;
	ls_t *sb = (ls_t *)b;
	
	return sa->time - sb->time;
}

void get_mtime()
{	
	qsort(array,count,sizeof(ls_t),compare);
	
	for(int i=0; i<count; i++)
		printf("%s   ",array[i].name);

	exit(0);
}

void get_path(char *path)	{	if(path[strlen(path)-1] != '/')	sprintf(path+strlen(path),"/");	}

void print_ldir(char *name)
{
	 printf("%c[1;41m",27);
     printf("%s",name);
     printf("%c[0m",27);
	 putchar('\n');

}

void print_dir(char *name)
{
	 printf("%c[1;31m",27);
     printf("%s",name);
     printf("%c[0m",27);
	 
	 putchar('\n');

}
char type(mode_t mode)
{ 
	if(S_ISREG(mode))	   return('-');
    if (S_ISDIR(mode))     return('d');
    if (S_ISCHR(mode))     return('c');
    if (S_ISBLK(mode))     return('b');
    if (S_ISLNK(mode))     return('l');
    if (S_ISFIFO(mode))    return('p');
    if (S_ISSOCK(mode))    return('s');

}

char* perm(mode_t mode) {
  
	int i;
    static char perms[10] = "--------";
	if(mode & S_IRUSR)	perms[0] = 'r';		else	perms[0] = '-';
	if(mode & S_IWUSR)	perms[1] = 'w';		else	perms[1] = '-';
	if(mode & S_IXUSR)	perms[2] = 'x';		else if(mode & S_ISUID)	perms[2] = 's';

	else	perms[2] = '-';
	if(mode & S_IRGRP)	perms[3] = 'r';		else	perms[3] = '-';
	if(mode & S_IWGRP)	perms[4] = 'w';		else	perms[4] = '-';
	if(mode & S_IXGRP)	perms[5] = 'x';		else if(mode & S_ISGID)	perms[5] = 's';

	else	perms[5] = '-';
	if(mode & S_IROTH)	perms[6] = 'r';		else	perms[6] = '-';
	if(mode & S_IWOTH)	perms[7] = 'w';		else	perms[7] = '-';
	if(mode & S_IXOTH)
	{
		if(mode & S_ISVTX)
			perms[8] = 't';
		else
			perms[8] = 'x';
	}else{
		if(mode & S_ISVTX)
			perms[8] = 'T';
		else
			perms[8] = '-';
	}

	
	perms[9]=0;
    
	return(perms);
}
int main(int argc, char **argv)
{
	int i,j;
    char path[PATH_LENGTH]="./";
    struct stat buf;
	if(access(argv[1],F_OK) == 0)
    {
        if(lstat(argv[1],&buf) == 0)
        {
            if((buf.st_mode & S_IFMT) == S_IFREG)
            {
                printf("%s \n",argv[1]);
                exit(0);
            }
        }
    }

    for(i=1;i<argc;i++)
    {
        for(j=1;argv[i][j] != '\0';j++)
        {
            if(argv[i][0] != '-')
            {
					strncpy(path,argv[i],PATH_LENGTH-1);
					break;
            }
			if(argc == 3) strncpy(path,argv[2],PATH_LENGTH-1);
            switch(argv[i][j])
            {
            case 'l':
                flag_mode |= 0x01;
                seek_dir(path,flag_mode);
                break;
            case 'i':
                flag_mode |= 0x02;
                seek_dir(path,flag_mode);
                break;
            case 't':
                flag_mode |= 0x04;
                seek_dir(path,flag_mode);
                break;
			default:
				printf("ls: illegal option -- %c\nusage: ls \'[-@ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1%%] [file ...]",argv[i][j]);
				exit(0);
            }
        }
    }
	get_path(path);
	seek_dir(path,flag_mode);
	
}
void seek_dir(char *dir,int opt) {
    
    struct dirent *entry;
    struct stat buf;
    DIR *dirp;
    
	int i=0;
    int ino;
	if(lstat(dir,&buf) == 0)
	{
		if(S_ISREG(buf.st_mode))
		{
			printstat(dir,&buf);
			exit(0);
		}
	}

	if((dirp = opendir(dir)) == NULL)
    {
        perror("open drectory error");
        exit(1);
    }
    chdir(dir);
	while((entry = readdir(dirp)) != NULL)
	{
		if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
            continue;
        if(lstat(entry->d_name, &buf) == 0)
		{	
			count++;
			total+=buf.st_blocks;
			strcpy(array[count].name,entry->d_name);
			array[count].time = buf.st_mtime;
		}
	}
	if(flag_mode == 0x01) printf("total  %d \n",total/2);
		rewinddir(dirp);
	while((entry = readdir(dirp)) != NULL)
    {
        if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
            continue;
        if(lstat(entry->d_name, &buf) == 0)
            printstat(entry->d_name,&buf);
    }
    
	closedir(dirp);
	exit(0);
}

void printstat(char *fname, struct stat *st)
{
	if(flag_mode == 0x00)
		color_change(st,fname);

    if(flag_mode == 0x01){
		total+=st->st_blocks;
        printf("%c%s ",type(st->st_mode), perm(st->st_mode));
        printf("%4lu ",st->st_nlink);
        printf("%s %s ",getpwuid(st->st_uid)->pw_name,getgrgid(st->st_gid)->gr_name);
        printf("%9ld ",st->st_size);
        printf("%.12s ",ctime(&st->st_mtime)+4);
		color_change(st,fname);    
    }
    if(flag_mode == 0x02)
    {
        printf("%9lu     ",st->st_ino);
		color_change(st,fname);
    }
    if(flag_mode == 0x04)
	{
		get_mtime();
	}
}
void color_change(struct stat *st, char *name)
{
	if(S_ISDIR(st->st_mode))
			print_dir(name);
    if(S_ISBLK(st->st_mode) || S_ISCHR(st->st_mode) || S_ISLNK(st->st_mode) ||
		S_ISFIFO(st->st_mode) || S_ISSOCK(st->st_mode))
			print_ldir(name);
	if(S_ISREG(st->st_mode))
            printf("%s  \n",name);
}
