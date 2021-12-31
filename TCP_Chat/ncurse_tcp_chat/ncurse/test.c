#include<string.h>
#include<ncurses.h>
#include<time.h>


char msg[1024];


char cur_time[20];
void get_time()
{
	time_t t;
	time(&t);

	struct tm *T;
	T=gmtime(&t);

	sprintf(cur_time,"%dmin %dsec ",T->tm_hour+9,T->tm_min);
}
	

void get_window()
{
	get_time();

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
#if 1
	panel = newwin(LINES - 5, COLS,0,0);
	box(panel,0,0);
	wrefresh(panel);
#endif
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

		
		sprintf(msg,"[TIT] : %s\n",buf);
		msg[strlen(msg)-1]='\0';
		wprintw(out,"%s",cur_time);
		wprintw(out,"%s\n",msg);

	//	mvwprintw(in,1,1,"%s",msg);
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


