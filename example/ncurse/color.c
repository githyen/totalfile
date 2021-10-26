#include<stdio.h>
#include "mecro.h"
#include<ncurses.h>


#define OUT(x...) fprintf(stderr,x)

int main(){

	char str[20];
	scanf("%s",str);

	initscr();

	int h,w,start_y,start_x;
	h=50; // window  높이 
	w=90; // window 넓이  
	start_y=start_x=0;  // 총 window 사이즈에서  표시할 위치 

	WINDOW *win = newwin(h,w,start_y,start_x); // 좌표를 설정한 위치 가져오기 

	refresh(); 
	box(win,0,0); // 사각형 그리기 

#if 1
	mvwprintw(win,1,3,"this is my box");
	mvwprintw(win,2,3,str);
	mvwprintw(win,3,3,"two:");
	mvwprintw(win,4,3,"three:");
	wrefresh(win);
#endif
//	sleep(2);
	int c= getch();
	endwin();
	return 0;
}
	
