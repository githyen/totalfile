#ifndef __matrix_h__
#define __matrix_h__

typedef struct{
		int r;
		int w;
		int **matrix;
		int **tmp;
}Graph;


Graph *init(int,int);
void addEdge(Graph *,int, int);


void get_data(char *fname);
void get_menu();
void print_data(Graph *);
void push_data(Graph *g, int n);
void get_matrix(int **, int i, int j, int count);
void set_process(int child, int get_mode);

void free_g(Graph *);
void show_matrix(Graph *);
#endif
