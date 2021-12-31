#include<stdio.h>
#include<sys/wait.h>
#include<fcntl.h>
#include "socket.h"

#define MAX_CLNT 256
#if 1
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // mutex init
pthread_cond_t cond =   PTHREAD_COND_INITIALIZER;		// cond init
#endif


static int clnt_cnt;
static int clnt_socks[MAX_CLNT];

void *control_clnt(void *);
void snd_msg(char *msg,int size);
void error_control(char *error);

int main(int argc, char **argv)
{
	char *port = argv[1];

	if(argc != 2) { OUT("<PORT> \n"); exit(EXIT_FAILURE); }
	int serv_sock,clnt_sock;
	struct sockaddr_in serv_addr,clnt_addr;

	socklen_t clnt_size;
	pthread_t tid;

	if(( serv_sock = socket(PF_INET,SOCK_STREAM,0)) == -1)
		error_control("socket () error");
	else OUT("SOCKET OK...\n");

	memset(&serv_addr,0,sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(port));

	if(bind(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1)
		error_control("bind () error");
	else OUT("BIND OK...\n");

	if(listen(serv_sock,5) == -1) error_control("listen () error");
	else OUT("listen ok...\n");

	while(1)
	{

		clnt_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_size);
		if(clnt_sock == -1) error_control("accept () error");
		else OUT("accept ok...\n");
		
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond,&mutex);
		clnt_socks[clnt_cnt++] = clnt_sock;
		pthread_mutex_unlock(&mutex);

		pthread_create(&tid,NULL,control_clnt,(void *)&clnt_sock);
		pthread_join(tid,NULL);
	}

	close(serv_sock);

	return 0;
}

void *control_clnt(void *arg)
{
	int sock=*((int *)arg);
	int str_len=0;
	char msg[buf_size]="";
	while((str_len=read(sock,msg,sizeof(msg)))!=0)
		snd_msg(msg,str_len);

	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&cond,&mutex);

	for(int i=0; i<clnt_cnt; i++)
	{
		if(sock == clnt_socks[i]) { 
			while(i++ <clnt_cnt-1) 
				clnt_socks[i]=clnt_socks[i+1]; 
			break; 
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutex);
	close(sock);
	
	return NULL;
}

void snd_msg(char *msg,int size)
{

	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&cond,&mutex);
	for(int i=0; i<clnt_cnt; i++)
		write(clnt_socks[i],msg,size);
	pthread_mutex_unlock(&mutex);

	return;
}
void error_control(char *error)
{
	fprintf(stderr,"%s",error);
	fprintf(stderr,"\n");
	exit(EXIT_FAILURE);
}

