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
