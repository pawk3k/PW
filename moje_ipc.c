#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX 10

int main(){
   int shmid, i;
   volatile int *buf;
   buf[1] = 1;
   shmid = shmget(45281, MAX*sizeof(int), IPC_CREAT|0600);
   if (shmid == -1){
      perror("Utworzenie segmentu pamieci wspoldzielonej");
      exit(1);
   }
   
   buf = (int*)shmat(shmid, NULL, 0);
   if (buf == NULL){
      perror("Przylaczenie segmentu pamieci wspoldzielonej");
      exit(1);
   }
    if(fork()==0){
    for (i=0; i<100; i++){
        while(buf[1]==1);
        printf("Numer: %5d   Wartosc: %5d\n", i, *buf);
        buf[1] = 1;
        sleep(1);
        }
    }else{
    for (i=0; i<100; i++){
      while(buf[1]==0);
      *buf = i;
      buf[1]=0;
      sleep(1);
    }
    }
    return 0;
}