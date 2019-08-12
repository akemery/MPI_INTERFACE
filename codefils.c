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
   int shmid, shmposid;
   msg_buff = 
        (struct message*)broker_createbuffer("shmbuff", 65, 0666, 1024, &shmid);
   pos = 
      (char *)broker_createbuffer("shmpos", 85, 
                        0666, 1024, &shmposid);
   while(TRUE){
       broker_sendmsg(semid, 0L, 0L, 0L, 0L, 0L, 0L, "toto", "titi");
   }
   //detache str de la mémoire partagée
   shmdt(msg_buff); 	
   return(0);
}
