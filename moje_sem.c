#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
//ipcrm -S 31
static struct sembuf buf;

void podnies(int semid, int semnum){
   buf.sem_num = semnum;
   buf.sem_op = 1;
   buf.sem_flg = 0;
   if (semop(semid, &buf, 1) == -1){
      perror("Podnoszenie semafora");
      exit(1);
   }
}

void opusc(int semid, int semnum){
   buf.sem_num = semnum;
   buf.sem_op = -1;
   buf.sem_flg = 0;
   if (semop(semid, &buf, 1) == -1){
      perror("Opuszczenie semafora");
      exit(1);
   }
}

int main(){
	int shmid;
   	volatile int *buf;
	shmid = shmget(45281, 1*sizeof(int), IPC_CREAT|0600);
	int i,j;
   	buf = (int*)shmat(shmid, NULL, 0);
	int semid1 = semget(31,1,IPC_CREAT|0666);
	int semid2 = semget(32,1,IPC_CREAT|0666);
	semctl(semid2,0,SETVAL,1);
	semctl(semid1,0,SETVAL,0);
	if(fork()==0){
	    for (i=0; i<100; i++){
		opusc(semid1,0);
		printf("Numer: %5d   Wartosc: %5d\n", i, *buf);

		podnies(semid2,0);
	}
	    }else{
	    for (i=0; i<100; i++){
		opusc(semid2,0);
		*buf = i;
		podnies(semid1,0);
		}

	    }
	    return 0;
}
