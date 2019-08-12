int broker_createsem(char *key_file, int key_id, int flags, int sem_size);
void *broker_createbuffer(char *key_file, int key_id, 
                          int flags, int shm_size, int *shmid);
int broker_sendmsg(int semid, long src, long dst, long count, long offset, 
          long opcode, long result, char *name, char *data);
int broker_lock(int semid);
int broker_unlock(int semid);
int broker_initsem(int semid);
void initialize_node(char* node_buff_file, char * broker_buff_file,
    char* node_info_buff_file, 
    int node_info_buff_id, int node_buff_id, int broker_buff_id,
    int node_info_buff_size, int node_buff_size, int broker_buff_size,
    int node_buff_flags, int node_info_buff_flags, int broker_buff_flags,
    int node_info_buff_shmid, int broker_buff_shmid, int node_buff_shmid );

struct message *broker_buff;
char *pos;
struct message *node_buff;

struct node_info{
   char *keyfile;
   int  keyid;
   int  in_buffid;
   int  out_buffid;
};

struct node_info *node_info_buff;


union semun {
   int  val;    /* Value for SETVAL */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short  *array;  /* Array for GETALL, SETALL */
   struct seminfo  *__buf;  /* Buffer for IPC_INF
                              (Linux-specific) */
};
