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
   struct message msg;
   msg.source = src;
   msg.dest = dst;
   msg.opcode = opcode;
   msg.count  = count;
   msg.offset = offset;
   msg.result = result;
   strncpy(msg.name, name, MAX_PATH);
   strncpy(msg.data, data, BUF_SIZE);
   int posid = atoi(pos);
   printf("Position id %d\n", posid);
   posid++;
   sprintf(pos, "%d", posid);
   broker_unlock(semid);
}

void initialize_node(char* node_buff_file, char * broker_buff_file,
    char* node_info_buff_file, 
    int node_info_buff_id, int node_buff_id, int broker_buff_id,
    int node_info_buff_size, int node_buff_size, int broker_buff_size,
    int node_buff_flags, int node_info_buff_flags, int broker_buff_flags,
    int node_info_buff_shmid, int broker_buff_shmid, int node_buff_shmid ){
   
    broker_buff = 
      (struct message*)broker_createbuffer(broker_buff_file, broker_buff_id, 
                   broker_buff_flags, broker_buff_size, &broker_buff_shmid);
    node_buff = 
      (struct message*)broker_createbuffer(node_buff_file, node_buff_id, 
                   node_buff_flags, node_buff_size, &node_buff_shmid);

    node_info_buff = 
      (struct message*)broker_createbuffer(node_info_buff_file, node_info_buff_id, 
               node_info_buff_flags, node_info_buff_size, &node_info_buff_shmid);
}
