#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX 10

struct buf_elem {
   long mtype;
   int mvalue;
};

#define PUSTY 1
#define PELNY 2

int main(){
   int msgid, i;
   struct buf_elem elem;
    
	if(fork()==0){
		msgid = msgget(45281, IPC_CREAT|0600);
	 	for(i = 0; i<100;i++){
		elem.mvalue = i;
		if(i%2==0){		
		elem.mtype = 7;
		msgsnd(msgid, &elem, sizeof(elem.mvalue), 0);
		}else{
		elem.mtype = 6;
	}	

		}
	}else{
		for(i =0;i<50;i++){
		msgid = msgget(45281, IPC_CREAT|0600);
		msgrcv(msgid, &elem, sizeof(elem.mvalue), 7, 0);
		printf("%d\n",elem.mvalue);
		}
	}
   
}
