#ifndef __head_h__
#define __head_h__
#include <sys/types.h>  
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h> 
#include <dirent.h> 
#include <pwd.h> 
#include <grp.h> 

#define PATH_LENGTH 200

void printstat(char *fname, struct stat *st);
void print_dir(char *name);
void print_ldir(char *name);

void get_path(char *path);
void get_mtime();

void seek_dir(char *dir, int opt);
void color_change(struct stat *st, char *name);

char type(mode_t mode);
char* perm(mode_t mode) ;

static int flag_mode = 0x00, count = 0;
static char *name[200];
static int total = 0;


typedef struct{
	char name[20];
	time_t time;
}ls_t;

ls_t array[200];


#endif
