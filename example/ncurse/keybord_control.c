#include<curses.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv)
{

	initscr();
	noecho();
	char c;

	while(c=getch())
	{
		mvprintw(1,0,"KEY : %s - 0x%02x\n",keyname(c),c);
	}
	endwin();

	return 0;
}
