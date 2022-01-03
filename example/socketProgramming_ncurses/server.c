#include<stdio.h>
#include<ncurses.h>
#include "socket.h"

#if 0
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // mutex init
pthread_cond_t cond =   PTHREAD_COND_INITIALIZER;		// cond init
#endif

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
	else OUT("socket ok...\n");

	memset(&serv_addr,0,sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1)
		error_control("bind () error");
	else OUT("bind ok...\n");

	if(listen(serv_sock,5) == -1) error_control("listen () error");
	else OUT("listen ok...\n");
	clnt_size = sizeof(clnt_addr);

	clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_size);
	if(clnt_sock == -1) error_control("accept () error");
	else OUT("accept ok...\n");


	pdata.sock = clnt_sock;
//	memcpy(pdata.user_name,argv[2],strlen(argv[2]+1));
	sprintf(pdata.user_name,"%s",argv[2]);
	get_window(argv[2]);
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
	
	char msg[msg_size]="";
	char buf[buf_size]="";
	char my_msg[msg_size*2]="";
	char *encrypted = NULL;

	while(1)
	{
		mvwprintw(in,1,1,"ENTER:");
		mvwgetstr(in,1,7,msg);
		/* input 데이터가 "exit"와 같으면 window 창 닫고 
		   종료 프로토콜 때문에 "exit"라는 데이터를 보고 프로그램 종료 */	
		if(strncmp(msg,"exit",4)==0)
		{
			endwin();
			write(pdata.sock,msg,strlen(msg)); 
			get_exit(NULL,pdata.sock); 
		}

		else{
			/* buf라는 공간에 user 이름과 입력 받은 msg 데이터를 저장 */
			sprintf(buf,"[%s] : %s\n",pdata.user_name,msg);
			sprintf(my_msg,"[ME] : %s\n",msg); /* 나의 화면에 출력하기 위해 저장 */

			buf[strlen(buf)-1]='\0';
			my_msg[strlen(my_msg)-1]='\0';

			/* 데이터를 암호화 하기 위해 동적할당 한 다음 할당한 공간에 Encrypt 함수로 
			   암호화 한 데이터를 복사한다 */
			encrypted = malloc(strlen(buf));
			memcpy(encrypted,Encrypt(set_key,buf,strlen(buf)),strlen(buf));

			/*나의 out window 창에 출력 */
			wprintw(out,"%s\n",my_msg);
			/* 암호화 된 데이터를 소켓을 통해 보냄 */
			write(pdata.sock,encrypted,strlen(encrypted));
			/* 나의 out window를 갱신 */
			wrefresh(out);

			free(encrypted);
		}

	}

	return NULL;
}
void *rcv_msg(void *get_sock)
{
	
	User pdata = *((User *)get_sock);
	char msg[buf_size];
	char *decrypted = NULL;
	int get_len;	

	while(1)
	{
		get_len = read(pdata.sock,msg,buf_size);
		
		if(get_len == -1)	 { 
			OUT("read () error\n"); 
			close(pdata.sock); 
			endwin();
			return NULL; 
		}

		/* 종료 프로토콜을 위해 "exit라는 문자열을 받았을 때 같으면 
		   window 닫고 프로그램 종료 */
		if(strncmp(msg,"exit",4)==0) { 
			
			endwin();
			get_exit(pdata.user_name,pdata.sock);
		}

		/* 받은 데이터를 복호화 하기 위해 동적할당한 공간에
		   Decrypt 함수를 통해 복호화 한 다음 복사 */
		decrypted = malloc(strlen(msg));
		memcpy(decrypted,Decrypt(set_key,msg,strlen(msg)),strlen(msg));

		decrypted[get_len]='\0';
		/* 복호화 한 데이터를 out window 에 출력 및 갱신 */
		wprintw(out,"%s\n",decrypted);
		wrefresh(out);
		free(decrypted);
	}

	return NULL;
}
void error_control(char *error)
{
	fprintf(stderr,"%s",error);
	fprintf(stderr,"\n");
	return;
}
