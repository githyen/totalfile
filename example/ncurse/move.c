#include<stdio.h>
#include<ncurses.h>
#include<unistd.h>

int timer()
{
	int r,c;
	r=c=10;
	initscr();
	noecho();

	int h,w,start_y,start_x;
	h=50;
	w=90;
	start_y=start_x=0;
	WINDOW *win = newwin(h,w,start_y,start_x);

	refresh();
	box(win,0,0);

	curs_set(FALSE);
	keypad(stdscr,TRUE);

	while(1)
	{
		int input=getch();
		clear();
		switch(input)
		{
			case KEY_UP:
				mvprintw(--r,c,"O");
		wrefresh(win);
				continue;
			case KEY_DOWN:
				mvprintw(++r,c,"O");
		wrefresh(win);
				continue;
			case KEY_LEFT:
				mvprintw(r,--c,"O");
		wrefresh(win);
				continue;
			case KEY_RIGHT:
				mvprintw(r,++c,"O");
		wrefresh(win);
				continue;
		}
		if(input=='q') break;
	}
	endwin();
	return 0;
}

int main(){

	return timer();
}
