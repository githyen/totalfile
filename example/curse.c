#include<stdio.h>
#include<stdlib.h>
#include "mecro.h"
#include<ncurses.h>
#include<unistd.h>
#include<pthread.h>

#define OUT(x...) fprintf(stderr,x)


void *set_thread(void *arg);

int main(){


        pthread_t t;

#if 1

        initscr();
        cbreak();
        noecho();

        int h,w,start_y,start_x;
        h=30; // window  높이
        w=50; // window 넓이
        start_y=start_x=0;  // 총 window 사이즈에서  표시할 위치
        if(pthread_create(&t,NULL,set_thread,NULL)!=0)
        {
                perror("create");
                exit(1);
        }
        WINDOW *win = newwin(h,w,start_y,start_x); // 좌표를 설정한 위치 가져오기

        refresh();
//      box(win,97,97); // 사각형 그리기  97 == 아스키코드 값

        int left,right,top,bottom,tlc,trc,blc,brc;

        left=right=99;
        top=0;
        bottom = 0;

        /* 공백으로 한 면을 그릴경우
           변수 = (int)space; 대입
        */
        tlc = trc = blc = brc = (int)'+';

        wborder(win,left,right,top,bottom,tlc,trc,blc,brc); // 가로 세로에 적절한 도형 그리기
        refresh();
        wrefresh(win);
        for(int i=2; i<9; i++)
        {
#if 1
                mvwprintw(win,i,5,"this is my box");
                wrefresh(win);
                sleep(1);
        }
#endif
        endwin();
        pthread_join(t,NULL);
#endif
        system("clear");
        return 0;
}

void *set_thread(void *arg)
{
        initscr();
        noecho();
        cbreak();

        int x,y;
        getmaxyx(stdscr,y,x);
        WINDOW *in = newwin(3,x-12,y-5,5);
        box(in,0,0);
        refresh();
        wrefresh(in);
        mvwprintw(in,1,1,"this is sume text\n");
        wrefresh(in);
        endwin();
        return NULL;
}