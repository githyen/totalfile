#include"head.h"


void get_data();

int main(){

	rcv_buf sdata;

	snd_buf rdata;
	key_t key;

	key=ftok("head.h",1024);

	int id;

	if((id=msgget(key,0644|IPC_CREAT))==-1)
	{
		perror("msgget");
		exit(1);
	}

	sdata.msg_type=1;

	printf("enter lines of text, \"end\" quit\n");

	while(1)
	{
		printf("나 :");
		fgets(sdata.data,sizeof(sdata.data),stdin);

		int len=strlen(sdata.data);

		if(sdata.data[len-1]=='\n')
			sdata.data[len-1]='\0';

		if(strcmp(sdata.data,"end")==0) break;

		if(msgsnd(id,&sdata,sizeof(sdata)-sizeof(long),0)==-1)
			fprintf(stderr,"msgsnd error \n");
// ----------------------------rcv----------------------------------------

		if(msgrcv(id,&rdata,sizeof(rdata.data),2,0)==-1)
		{
			fprintf(stderr,"채팅창을 닫습니다. \n");
			sleep(1);
			system("clear");
			exit(EXIT_FAILURE);
		}
		printf("상대방 : \%s\"\n",rdata.data);

	}
	if(msgctl(id,IPC_RMID,NULL) == -1)
	{
		fprintf(stderr,"msgctl error \n");
		exit(1);
	}

	exit(0);
}

