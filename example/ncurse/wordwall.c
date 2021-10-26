#include <ncurses.h>
#include "mecro.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>


void *set_data(void *arg);

void print()	{

	char *str="this test text";
	initscr();
//	color_start();

	int h,w,start_y,start_x;
	h=10; // 높이
	w=40; // 넓이 
	start_y=start_x=25.0;

	WINDOW *win = newwin(h,w,start_y,start_x);
	refresh();
	box(win,0,0);
	
	for(int i=1; i<=5; i++)
	{
		mvwprintw(win,i,3,"%s",str);
	}
	wrefresh(win);
	endwin();
}

int main(int argc, char **argv)
{
	pthread_t tid;

	print();
	if(pthread_create(&tid,NULL,set_data,NULL)!=0)
	{
		perror("create");
		exit(1);
	}

	pthread_join(tid,NULL);
	return 0;
}

void *set_data(void *arg)
{

	// curses start
	initscr();
	noecho();
	cbreak();

	print();
	//get screen size
	int x,y;
	getmaxyx(stdscr,y,x);

	// create a window for our input

	WINDOW *input = newwin(6,x-12,y-8,5);
	box(input,0,0);
	refresh(); // monitor out
	wrefresh(input);

	keypad(input,true);

	/* file에서 가져온 데이터 출력 예정 */
	char *text[3]={"one","two","three"};
	int ch,high=0;
	while(1)
	{
		for(int i=0; i<3; i++)
		{
			if(i == high)
				wattron(input,A_REVERSE);
			mvwprintw(input,i+1,1,text[i]);
			wattroff(input,A_REVERSE);
		}
		ch = wgetch(input);
		switch(ch)
		{
			case KEY_UP:
				high--;
				if(high == -1)
					high = 0;
				break;
			case KEY_DOWN:
				high++;
				if(high == 3)
					high = 2;
				break;
			default:
				break;
		}

		if(ch == 10)
			break;
	}


	mvprintw(40,40,"your choice was : %s \n",text[high]);

	// make sure program waits before exiting ...
	getch();
	endwin();
	//curses end

	return NULL;
}

