#include<stdio.h>
#include"socket.h"

#if 1
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
#endif

void error_control(char *error);
void *rcv_msg(void *);
void *snd_msg(void *);

char name[40]="NULL";
char msg[buf_size];

int main(int argc, char **argv)
{

	if(argc != 3)
	{
		fprintf(stderr,"<PORT> <User_Name>\n");
		exit(EXIT_FAILURE);
	}
	char *ip = "127.0.0.1";
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t rcv_thread;
	pthread_t snd_thread;
	void *t_return=NULL;


	sprintf(name,"[%s]",argv[2]);

	if(( sock = socket(PF_INET,SOCK_STREAM,0)) == -1)
		error_control("socket () error");

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(atoi(argv[1]));
	
	if(connect(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1)
		error_control("connect () error");
	else OUT("connect ok ...\n");

//	pdata.sock = sock;
//	memcpy(pdata.user_name,argv[2],strlen(argv[2])+1);
//	sprintf(pdata.user_name,"%s",argv[2]);
	pthread_create(&snd_thread,NULL,snd_msg,(void *)&sock);
	pthread_create(&rcv_thread,NULL,rcv_msg,(void *)&sock);

	pthread_join(snd_thread,&t_return);
	pthread_join(rcv_thread,&t_return);

	close(sock);

	return 0;

}
void *snd_msg(void *get_sock)
{	
	int sock = *((int *)get_sock);
	char buf[buf_size];
	while(1)
	{
		fgets(buf,buf_size,stdin);
		
		if(strncmp(buf,"exit",4)==0) {
			write(sock,buf,strlen(buf));
			get_exit(NULL,sock);
		
		}
		sprintf(buf,"[%s] : %s",name,msg);
		write(sock,buf,strlen(buf));
	}

	return NULL;
}

void *rcv_msg(void *get_sock)
{
	int sock = *((int *)get_sock);
	char buf[buf_size];
	int get_len;
	while(1)
	{
		get_len = read(sock,buf,buf_size);
		
		if(get_len == -1)			 { OUT("종료합니다\n"); close(sock); exit(0); }
		
		buf[get_len]=0;
		OUT("%s",buf);

	}
	return NULL;
}

void error_control(char *error)
{
	fprintf(stderr,"%s",error);
	fprintf(stderr,"\n");
	exit(EXIT_FAILURE);
}
