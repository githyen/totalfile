#ifndef __head_h__
#define __head_h__
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<pthread.h>


typedef struct{

	long msg_type;
	char data[1024];
}snd_buf;


typedef struct{
	long msg_type;
	char data[1024];
}rcv_buf;


#endif

