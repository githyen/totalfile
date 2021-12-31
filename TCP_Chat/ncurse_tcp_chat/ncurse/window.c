#include<string.h>
#include<ncurses.h>

void get_window()
{

	WINDOW *in;
	WINDOW *out;
	WINDOW *panel;

	int i;

	char buf[100];

	initscr();
	refresh();

	out = newwin(LINES - 7, COLS -1,1,1);
	scrollok(out,TRUE);
	box(out,0,0);
	wrefresh(out);

	panel = newwin(LINES - 5, COLS,0,0);
	box(panel,0,0);
	wrefresh(panel);

	in = newwin(5,COLS,LINES-5,0);
	box(in,0,0);
	wrefresh(in);

#if 1
	char *data="ENTER:";
	while(1)
	{
		mvwprintw(in,1,1,"ENTER:");
		mvwgetstr(in,1,7,buf);
	//	wclear(in);
		if(NULL != strstr(buf,"exit")) break;

		wprintw(out,"%s\n",buf);
#if 0
		for(int i=0; i<strlen(buf); i++)
			buf[i] = 0x20;
#endif
		mvwprintw(in,1,1,"%s",buf);
		wrefresh(out);
		wrefresh(in);
	}
#endif
	endwin();
	return ;
}

int main()
{
	get_window();
	return 0;
}


