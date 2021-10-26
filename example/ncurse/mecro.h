#ifndef __CHEAD__
#define __CHEAD__

#define OUT(x...) fprintf(stderr,x) // standard OUT
#define IN(x...) fscanf(stdin,x) // standard IN

#define swap(type,x,y) do { type t=x; x=y; y=t; } while(0) //swap
#define parent(x) (x-1)/2 

#define upper(c) ((c)-0x20)  // 소문자변환
#define lower(c) ((c)+0x20) // 대문자변환

#define  ARRAY_SIE(x) (sizeof((x))/ sizeof((x)[0])) // array sie
#define ABS(x) (((x)<0) ?-(x):(x)) // 절대값
#define MAX(x,y) ((x)>(y) ? (x):(y))
#define MIN(x,y) ((x)<(y) ? (x):(y))

#define C_NRM "\x1B[0m"
#define C_RED "\x1B[1;31m"
#define C_GRN "\x1B[1;32m"
#define C_YEL "\x1B[1;33m"
#define C_BLU "\x1B[1;34m"
#define C_MAG "\x1B[1;35m"
#define C_CYN "\x1B[1;36m"
#define C_WHT "\x1B[1;37m"


/* C file descripter 
   FILE type's   : FILE *fp; ->  int fd=fileno(fp);
*/

#endif
