#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "header.h"

#include "broker.h"


int main(){  
   int posid = 0; 
   // on demande au system de nous créer le semaphore
   int semid = broker_createsem("semfile", 75, 0666|IPC_CREAT , 1);

   if(semid < 0)
      printf("Impossible de créer le semaphore\n");
   broker_initsem(semid);
   int shmid, shmposid;

   initialize_node("shmnodebuff", "shmbuff", "shmnodeinfobuff", 45, 55, 65,
    1024, 1024, 1024, 0666|IPC_CREAT, 0666|IPC_CREAT, 0666|IPC_CREAT, 
    &node_infoshmid, &shmid, &node_shmid );
   // lie la memoire partagée a str2 
   msg_buff = 
      (struct message*)broker_createbuffer("shmbuff", 65, 
                        0666|IPC_CREAT, 1024, &shmid);
   if(msg_buff == NULL)
       printf("pointeur null\n");
   pos = 
      (char *)broker_createbuffer("shmpos", 85, 
                        0666|IPC_CREAT, 1024, &shmposid);
   if(pos == NULL)
      printf("pointeur null 2\n");

   /* Init the position in the buffer*/
   broker_lock(semid);
      sprintf(pos, "%d", posid);
   broker_unlock(semid);
   
   while(TRUE){
      broker_sendmsg(semid, 0L, 0L, 0L, 0L, 0L, 0L, "papa", "toto");
   }
   
   // lecture de la valeur finale
   printf("Valeur Finale de s = %ld\n", msg_buff->source);
   //detacche str de la mémoire partagée
   shmdt(msg_buff); 
   shmdt(pos); 
   // detruit la mémoire partagée 
   shmctl(shmid,IPC_RMID,NULL);
   shmctl(shmposid,IPC_RMID,NULL);
   // des truction du semaphore
   if(semctl(semid, 0, IPC_RMID, NULL) == -1){
       perror("semctl");
       return(-1);
   }	
}
