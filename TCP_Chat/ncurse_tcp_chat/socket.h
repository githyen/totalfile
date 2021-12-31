#ifndef __SOCK_H__
#define __SOCK_H__

#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>
#include<ncurses.h>

#define buf_size 1024
#define OUT(x...) fprintf(stderr,x)

typedef struct{

	int sock;
	char user_name[50];

}User;

static WINDOW *in;
static WINDOW *out;
static WINDOW *panel;

void get_window()
{

    char buf[1024];

    initscr();
    refresh();
    out = newwin(LINES - 7,COLS-1,1,1);
    scrollok(out,TRUE);
    box(out,0,0);
    wrefresh(out);

    panel = newwin(LINES - 5, COLS,0,0);
    box(panel,0,0);
    wrefresh(panel);

    in = newwin(5,COLS,LINES-5,0);
    box(in,0,0);
    wrefresh(in);

    
}

void get_exit(char *user_name,int sockfd)
{
	if(user_name!=NULL) {
		OUT("%s님이 퇴장했습니다. 종료합니다\n",user_name);
		close(sockfd);
		exit(0);
	}
	else{
		OUT("종료합니다\n");
		close(sockfd);
		exit(0);
	}
}
	
#endif
