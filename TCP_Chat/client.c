#include<stdio.h>
#include"socket.h"

#if 0
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
#endif

void error_control(char *error);
void *rcv_msg(void *);
void *snd_msg(void *);

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

	User pdata;

	
	if(( sock = socket(PF_INET,SOCK_STREAM,0)) == -1)
		error_control("socket () error");

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(atoi(argv[1]));
	
	if(connect(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1)
		error_control("connect () error");
	else OUT("connect ok ...\n");

	pdata.sock = sock;
	memcpy(pdata.user_name,argv[2],strlen(argv[2])+1);

	pthread_create(&snd_thread,NULL,snd_msg,(void *)&pdata);
	pthread_create(&rcv_thread,NULL,rcv_msg,(void *)&pdata);

	pthread_join(snd_thread,NULL);
	pthread_join(rcv_thread,NULL);

	close(sock);

	return 0;

}
void *snd_msg(void *data)
{	
	User pdata = *((User *)data);
	char buf[buf_size];
	while(1)
	{
		OUT("%s:",pdata.user_name);
		fgets(buf,buf_size,stdin);
		
		if(strncmp(buf,"exit",4)==0) {
			write(pdata.sock,buf,strlen(buf));
			get_exit(NULL,pdata.sock);
		}
	
		write(pdata.sock,buf,strlen(buf));
	}

	return NULL;
}

void *rcv_msg(void *data)
{
	User pdata = *((User *)data);
	char buf[buf_size];
	int get_len;
	while(1)
	{
		get_len = read(pdata.sock,buf,buf_size);
		
		if(get_len == -1)			 { OUT("종료합니다\n"); return NULL; }
		
		if(strncmp(buf,"exit",4)==0) get_exit(pdata.user_name,pdata.sock);

		buf[get_len]='\0';

		OUT("\nserver:");
		write(1,buf,get_len);
	}
	return NULL;
}

void error_control(char *error)
{
	fprintf(stderr,"%s",error);
	fprintf(stderr,"\n");
	exit(EXIT_FAILURE);
}
