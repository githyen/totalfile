#include<stdio.h>
#include<ncurses.h>
#include "socket.h"
#if 0
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // mutex init
pthread_cond_t cond =   PTHREAD_COND_INITIALIZER;		// cond init
#endif

static WINDOW *in;
static WINDOW *out;
static WINDOW *panel;

void get_window(User pdata)
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
void error_control(char *error);
void *snd_msg(void *);
void *rcv_msg(void *);
int main(int argc, char **argv)
{
	char *port = argv[1];

	if(argc != 3) { OUT("<PORT> <User_Name> \n"); exit(EXIT_FAILURE); }

	int serv_sock,clnt_sock;

	struct sockaddr_in serv_addr,clnt_addr;

	User pdata;
	
	socklen_t clnt_size;

	pthread_t rcv_thread;
	pthread_t snd_thread;

	if(( serv_sock = socket(PF_INET,SOCK_STREAM,0)) == -1)
		error_control("socket () error");
	else OUT("SOCKET OK...\n");

	memset(&serv_addr,0,sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1)
		error_control("bind () error");
	else OUT("BIND OK...\n");

	if(listen(serv_sock,5) == -1) error_control("listen () error");
	else OUT("listen ok...\n");
	clnt_size = sizeof(clnt_addr);

	clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_size);
	if(clnt_sock == -1) error_control("accept () error");
	else OUT("accept ok...\n");

	pdata.sock = clnt_sock;

//	memcpy(pdata.user_name,argv[2],strlen(argv[2]+1));
	sprintf(pdata.user_name,"%s",argv[2]);
	get_window(pdata);
	pthread_create(&snd_thread,NULL,snd_msg,(void *)&pdata);
	pthread_create(&rcv_thread,NULL,rcv_msg,(void *)&pdata);

	pthread_join(snd_thread,NULL);
	pthread_join(rcv_thread,NULL);

	close(clnt_sock);
	close(serv_sock);

	endwin();
	exit(0);
}

void *snd_msg(void *get_sock)
{
	User pdata = *((User *)get_sock);
	
	char msg[buf_size]="";
	while(1)
	{
		mvwprintw(in,1,1,"ENTER:");
		mvwgetstr(in,1,7,msg);
		
		if(NULL != strstr(msg,"exit")) { 

			write(pdata.sock,msg,strlen(msg)); 
			get_exit(NULL,pdata.sock); 
		}
	
		wprintw(out,"%s\n",msg);
		write(pdata.sock,msg,strlen(msg));
		mvwprintw(in,1,1,"%s",msg);
		wrefresh(in);

	}

	return NULL;
}
void *rcv_msg(void *get_sock)
{
	
	User pdata = *((User *)get_sock);
	char msg[buf_size];
	int get_len;	

	while(1)
	{
		get_len = read(pdata.sock,msg,buf_size);
		
		if(get_len == -1)	 { OUT(" rcv read error\n"); close(pdata.sock); return NULL; }

		if(strncmp(msg,"exit",4)==0)  get_exit(pdata.user_name,pdata.sock);

		msg[get_len]='\0';
	
		wprintw(out,"%s\n",msg);
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
