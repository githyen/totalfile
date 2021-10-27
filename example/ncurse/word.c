#include "header.h"
#include <ncurses.h>
#include <pthread.h>

#define Enter 0x0a
#define ESC 0x1b

#define RIGHT 0x02
void *set_data(void *arg);
void get_data(char *fname);

void print(int size)	{

	initscr();

	int h,w;
	h=10; // 높이
	w=50; // 넓이 

	int x,y;

	getmaxyx(stdscr,y,x);

	/* top box */
	WINDOW *win = newwin(h,w,y-40,25.0);

	refresh();
	box(win,0,0);
	
	for(int i=1; i<=5; i++)
		mvwprintw(win,i,3,"%s",W[i].Eng);


	mvwprintw(win,6,3,"monitor x,y pos %d %d",x,y);


	wrefresh(win);
	endwin();
}

int main(int argc, char **argv)
{
	
	pthread_t tid;

	get_data(argv[1]);
	
	if(pthread_create(&tid,NULL,set_data,NULL)!=0)
	{
		perror("create");
		exit(1);
	}

	pthread_join(tid,NULL);
	return 0;
}

void get_data(char *fname){
	
	FILE *fp=NULL;

	if((fp=fopen(fname,"rt"))==NULL)
	{
		perror("open");
		exit(1);

	}
	
		int i=0;
		while(fscanf(fp,"%s %s",W[i].Eng,W[i].Kor)!=EOF) i++;
		fclose(fp);
		rewind(stdout);
		fp=NULL;
		print(i);

}
	
void *set_data(void *arg)
{

	// curses start
	initscr();
	noecho();
	cbreak();

//	print();
	//get screen size
	int x,y;
	getmaxyx(stdscr,y,x);

	// create a window for our input

	/* bottom box */
	WINDOW *input = newwin(6,x-12,y-30,6);
	box(input,0,0);
	refresh(); // monitor out
	wrefresh(input);

	curs_set(FALSE);
	keypad(input,true);

	/* file에서 가져온 데이터 출력 예정 */
	char *text[3]={"one","two","three"};
	int ch,high=0;

	short pos_y[6]={1,10,20,30,40,50};
	while(1)
	{
		for(int i=0; i<3; i++)
		{
			if(i == high)
				wattron(input,A_REVERSE);
			mvwprintw(input,2,pos_y[i],text[i]);
			wattroff(input,A_REVERSE);
		}
		ch = wgetch(input);
		switch(ch)
		{
			case KEY_LEFT:
				high--;
				if(high == -1)
					high = 0;
				break;
			case KEY_RIGHT:
				high++;
				if(high == 3)
					high = 2;
				break;
			default:
				break;
		}

		if(ch == Enter)
			break;
	}

	mvprintw(7,35,"your choice was : %s \n",text[high]);

	// make sure program waits before exiting ...
	getch();
	endwin();
	//curses end

	return NULL;
}

