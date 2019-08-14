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

   struct broker_shm_info broker_buff_info = {"broker_buff",65,20480,0666|IPC_CREAT,2};
   struct broker_shm_info node_buff_info = {"node_buff",75,1024,0666|IPC_CREAT,2};
   struct broker_shm_info node_info_buff_info = {"node_info_buff",95,1024,0666|IPC_CREAT,2};
   initialize_node(broker_buff_info, node_buff_info, node_info_buff_info );

   // lie la memoire partagée a str2 
   pos = 
      (char *)broker_createbuffer("shmpos", 85, 
                        0666|IPC_CREAT, 1024, &shmposid);
   if(pos == NULL)
      printf("pointeur null 2\n");

   /* Init the position in the buffer*/
   broker_lock(semid);
      sprintf(pos, "%d", posid);
   broker_unlock(semid);
   int i;
   for(i=0; i<8; i++){
      broker_sendmsg(semid, 0L, 0L, 0L, 0L, 0L, 0L, "papa", "toto");
   }
   
   while(1);
   // lecture de la valeur finale
   printf("Valeur Finale de s = %ld\n", broker_buff->source);
   //detacche str de la mémoire partagée
   shmdt(broker_buff);
   shmdt(node_buff); 
   shmdt(node_info_buff);
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
