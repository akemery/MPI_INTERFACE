#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "header.h"
#include "broker.h"

int main(){
   printf("Je suis un fils indépendant de son père %d\n", getpid());
   // generation de la clé pour la mémoire partagée
   
   int s = 0; // shared data
   int i;  
   // on demande au system de nour créer le semaphore
   int semid = broker_createsem("semfile", 75, 0666, 1);
   
   // lie la memoire partagée a str2 
   int shmposid;
   struct broker_shm_info broker_buff_info = {"broker_buff",65,20480,0666,2};
   struct broker_shm_info node_buff_info = {"node_buff",75,1024,0666,2};
   struct broker_shm_info node_info_buff_info = {"node_info_buff",95,1024,0666,2};
   initialize_node(broker_buff_info, node_buff_info, node_info_buff_info );
   pos = 
      (char *)broker_createbuffer("shmpos", 85, 
                        0666, 1024, &shmposid);
   for(i=0; i<4; i++){
       broker_rcvmsg(semid);
   }
   //detache str de la mémoire partagée
   shmdt(broker_buff);
   shmdt(node_buff); 
   shmdt(node_info_buff);
   shmdt(pos);  	
   return(0);
}
