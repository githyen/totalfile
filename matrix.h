#ifndef __matrix_h__
#define __matrix_h__

typedef struct{
		int r;
		int w;
		int n;
		int **matrix;
		int **tmp;
}Graph;


Graph *init(int,int);
void free_g(Graph *);
#endif
