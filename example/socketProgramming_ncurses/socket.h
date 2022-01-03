#ifndef __SOCK_H__
#define __SOCK_H__

#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>
#include<ncurses.h>
#include<openssl/des.h>

#define buf_size 1024
#define msg_size 100
#define OUT(x...) fprintf(stderr,x)

typedef struct{

	int sock;
	char user_name[20];

}User;
/* ncurse window */
static WINDOW *in;
static WINDOW *out;
static WINDOW *p;
static WINDOW *user_list;

/* data encrypt end decrypt */
static char *get_encrypt;
static char *get_decrypt;
static char set_key[]="server&client";
char * Encrypt( char *Key, char *Msg, int size);   
char * Decrypt( char *Key, char *Msg, int size); 

/* get_window and exit */
void get_window(char *Name);
void get_exit(char *user_name,int sockfd);

char * Encrypt( char *Key, char *Msg, int size) {   

	int n=0; 
	DES_cblock Key2; 
	DES_key_schedule schedule;   
	
	get_encrypt = ( char * ) malloc( size );
	/* Prepare the key for use with DES_cfb64_encrypt */ 

	memcpy( Key2, Key,8); 
	DES_set_odd_parity( &Key2 ); 
	DES_set_key_checked( &Key2, &schedule );   
	
	/* Encryption occurs here */ 
	DES_cfb64_encrypt( ( unsigned char * ) Msg, ( unsigned char * ) get_encrypt, size, &schedule, &Key2, &n, DES_ENCRYPT );   
	
	return (get_encrypt); 
}     

char * Decrypt( char *Key, char *Msg, int size) {   
	
	int n=0;   
	DES_cblock Key2; 
	DES_key_schedule schedule;   
	
	get_decrypt = ( char * ) malloc( size );   
	/* Prepare the key for use with DES_cfb64_encrypt */ 
	
	memcpy( Key2, Key,8); 
	DES_set_odd_parity( &Key2 ); 
	DES_set_key_checked( &Key2, &schedule );   
	
	/* Decryption occurs here */ 
	DES_cfb64_encrypt( ( unsigned char * ) Msg, ( unsigned char * ) get_decrypt, size, &schedule, &Key2, &n, DES_DECRYPT );   
	
	return (get_decrypt);   
}

void get_window(char *Name)
{


    initscr();
    refresh();

	user_list = newwin(LINES-41,COLS-1,1,0);
	box(user_list,0,0);
	wrefresh(user_list);

    out = newwin(LINES - 17,COLS-1,11,1);
    scrollok(out,TRUE);
    box(out,0,0);
    wrefresh(out);

    p = newwin(LINES - 5, COLS,9,0);
    box(p,0,0);
    wrefresh(p);

    in = newwin(5,COLS,LINES-5,0);
    box(in,0,0);
    wrefresh(in);

	mvwprintw(user_list,1,1," [USER_LIST]");
	mvwprintw(user_list,2,1," -%s-",Name);
	wprintw(out,"------------------------------\n");
	wprintw(out,"---Close Chat Window (exit)---\n");
	wprintw(out,"------------------------------\n");
	wrefresh(out);
	wrefresh(user_list);
    
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
