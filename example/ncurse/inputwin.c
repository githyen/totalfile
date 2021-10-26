#include <ncurses.h>
#include <stdio.h>
#include <string.h>


void *set_word(void *arg);

void print()
{
	printw("THIS IS SAME TEST \n");
}

int main(int argc, char **argv)
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


	printw("your choice was : %s \n",text[high]);

	// make sure program waits before exiting ...
	getch();
	endwin();
	//curses end

	return 0;
}
void *set_word(void *arg)
{
	initscr();
	noecho();
	cbreak();

	//get screen size
	int x,y;

	getmaxyx(stdscr,y,x);
	WINDOW *input = newwin(y/2,x/2,y/4,x/4);
	box(input,0,0);
	wgetch(input);
	endwin();
	return NULL;
}

