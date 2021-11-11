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
        h=30; // window  ����
        w=50; // window ����
        start_y=start_x=0;  // �� window �������  ǥ���� ��ġ
        if(pthread_create(&t,NULL,set_thread,NULL)!=0)
        {
                perror("create");
                exit(1);
        }
        WINDOW *win = newwin(h,w,start_y,start_x); // ��ǥ�� ������ ��ġ ��������

        refresh();
//      box(win,97,97); // �簢�� �׸���  97 == �ƽ�Ű�ڵ� ��

        int left,right,top,bottom,tlc,trc,blc,brc;

        left=right=99;
        top=0;
        bottom = 0;

        /* �������� �� ���� �׸����
           ���� = (int)space; ����
        */
        tlc = trc = blc = brc = (int)'+';

        wborder(win,left,right,top,bottom,tlc,trc,blc,brc); // ���� ���ο� ������ ���� �׸���
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