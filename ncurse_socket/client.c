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
//	memcpy(pdata.user_name,argv[2],strlen(argv[2])+1);
	sprintf(pdata.user_name,"%s",argv[2]);
	
	get_window(argv[2]);
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
	
	char buf[100]="";
	char msg[buf_size]="";
	char my_msg[200]="";


	while(1)
	{
		mvwprintw(in,1,1,"ENTER:");
		mvwgetstr(in,1,7,buf);
		
		if(strncmp(buf,"exit",4)==0) {
			endwin();
			write(pdata.sock,buf,strlen(buf));
			get_exit(NULL,pdata.sock);
		}
		else{
			sprintf(msg,"[%s] : %s\n",pdata.user_name,buf);
			sprintf(my_msg,"[ME] : %s\n",buf);
			msg[strlen(msg)-1]='\0';
			my_msg[strlen(my_msg)-1]='\0';
			wprintw(out,"%s\n",my_msg);
			write(pdata.sock,msg,strlen(msg));
			wrefresh(out);
		}
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
		
		if(get_len == -1)		 { 
			wprintw(out,"read () error\n"); 
			close(pdata.sock); 
			endwin();
			return NULL; 
		}
		
		if(strncmp(buf,"exit",4)==0) 
		{
			endwin();
			get_exit(pdata.user_name,pdata.sock);
		}

		buf[get_len]='\0';
		wprintw(out,"%s\n",buf);
		wrefresh(out);
	}

	return NULL;
}

void error_control(char *error)
{
	fprintf(stderr,"%s",error);
	fprintf(stderr,"\n");
	return;
}
