#include"head.h"

int main(){

	key_t key;

	rcv_buf rdata;

	snd_buf sdata;

	key=ftok("head.h",1024);

	int id;

	int len;

	if((id=msgget(key,0644|IPC_CREAT)) == -1)
	{
		fprintf(stderr,"msgget error \n");
		exit(1);
	}

	printf("spock : ready to receive message, captain\n");

	while(1)
	{
		if(msgrcv(id,&rdata,sizeof(rdata.data),1,0) == -1)
		{
			fprintf(stderr,"채팅창을 닫습니다. \n");
			sleep(2);
			system("clear");
			exit(EXIT_FAILURE);
		}
		printf("상대방 :\%s\"\n",rdata.data);

		sdata.msg_type=2;

		printf("enter lines of text, \"end\" quit\n");

		printf("나 :");
		fgets(sdata.data,sizeof(sdata.data),stdin);

			len=strlen(sdata.data);
			if(sdata.data[len-1]=='\n')
				sdata.data[len-1]='\0';

			if(strcmp(sdata.data,"end")==0) break;

			if(msgsnd(id,&sdata,sizeof(sdata)-sizeof(long),0)==-1)
				fprintf(stderr,"msgsnd error \n"); 
	}


	if(msgctl(id,IPC_RMID,NULL)==-1) { perror("msgctl"); exit(1); }

	exit(0);
}
