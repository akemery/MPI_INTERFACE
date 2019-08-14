#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>

#include "header.h"
#include "broker.h"

int broker_createsem(char *key_file, int key_id, int flags, int sem_size){
   key_t sem_key = ftok(key_file,key_id); 
   if(sem_key < 0){
     return(E_IO);
     perror("key");
   }
   int semid = semget(sem_key, sem_size, flags);
   if(semid < 0){
     return(E_IO);
     perror("semget");
   }
   return(semid);
}

void *broker_createbuffer(char *key_file, int key_id, int flags, 
                           int shm_size, int *shmid){
   key_t shm_key = ftok(key_file,key_id); 
   if(shm_key < 0)
     return(NULL);
   // SE crée la mémoire partagée 
   *shmid = shmget(shm_key,shm_size,flags);
   if(*shmid < 0)
     return(NULL);
   return((void*) shmat(*shmid,(void*)0,0));
}

int broker_lock(int semid){
   struct sembuf sb = {0, 0, 0}; 
   union semun sem_arg;
  // tentative de verrou du semaphore
  sb.sem_op = -1;
  if(semop(semid, &sb,1) == -1){
     perror("sempo");
     return(-1);
  }
  return(OK);
}

int broker_unlock(int semid){
  struct sembuf sb = {0, 0, 0}; 
  union semun sem_arg;
  sb.sem_op = 1;
  if(semop(semid, &sb,1) == -1){
     perror("sempo");
     return(-1);
  }
  return(OK);
}

int broker_initsem(int semid){
   union semun sem_arg;
   // la valeur du semaphore est initialisée à 1
   sem_arg.val = 1;
   if(semctl(semid, 0, SETVAL, sem_arg)==-1){
       perror("semctl");
       return(E_IO);
   }
   return(OK);
}

int broker_sendmsg(int semid, long src, long dst, long count, long offset, 
          long opcode, long result, char *name, char *data){
   broker_lock(semid);
   int posid = atoi(pos);
   (broker_buff+posid)->source = src;
   (broker_buff+posid)->dest = dst;
   (broker_buff+posid)->opcode = opcode;
   (broker_buff+posid)->count  = count;
   (broker_buff+posid)->offset = offset;
   (broker_buff+posid)->result = result;
   strncpy((broker_buff+posid)->name, name, MAX_PATH);
   strncpy((broker_buff+posid)->data, data, BUF_SIZE);
   posid++;
   sprintf(pos, "%d", posid);
   broker_unlock(semid);
}

int broker_rcvmsg(int semid){
   broker_lock(semid);
   int posid = atoi(pos);
   printf("recv Position id %d %s %s\n", posid, (broker_buff+posid)->name, 
                            (broker_buff+posid)->data );
   posid--;
   sprintf(pos, "%d", posid);
   broker_unlock(semid);
}

void initialize_node(struct broker_shm_info broker_buff_info,
                     struct broker_shm_info node_buff_info,
                     struct broker_shm_info node_info_buff_info ){
    broker_buff = 
      (struct message*)broker_createbuffer(broker_buff_info.shm_file,
                        broker_buff_info.shm_conf_id, 
                        broker_buff_info.shm_flags, 
                        broker_buff_info.shm_size, 
                        &broker_buff_info.shm_id);
    if(broker_buff == NULL)
         printf("Impossible de créer le buffer\n");
    node_buff = 
      (struct message*)broker_createbuffer(node_buff_info.shm_file,
                        node_buff_info.shm_conf_id, 
                        node_buff_info.shm_flags, 
                        node_buff_info.shm_size, 
                        &node_buff_info.shm_id);

    node_info_buff = 
      (struct node_info *)broker_createbuffer(node_info_buff_info.shm_file,
                        node_info_buff_info.shm_conf_id, 
                        node_info_buff_info.shm_flags, 
                        node_info_buff_info.shm_size, 
                        &node_info_buff_info.shm_id);
}

int register_node(struct node_info node){
   return(OK);
}
