#include"head.h"

int main(){

	int msgid;

	printf("삭제할 메세지큐 ID를 입력하세요 :");
	scanf("%d",&msgid);
	if(msgctl(msgid,IPC_RMID,NULL)==-1)
		perror("msgctl");

	int smid;
	printf("삭제할 세마포어 ID를 입력하세요 :");
	scanf("%d",&smid);

	if(semctl(smid,0,IPC_RMID)==-1)
		fprintf(stderr,"smctl error \n");

	exit(0);
}
