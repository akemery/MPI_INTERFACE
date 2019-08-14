struct message *broker_buff;
char *pos;
struct message *node_buff;

struct node_info{
   int node_id;
   struct message *node_buff;
   int node_buff_pos;
};

struct broker_shm_info{
   char *shm_file;
   int shm_conf_id;
   int shm_size;
   int shm_flags;
   int shm_id;
};

struct node_info *node_info_buff;


union semun {
   int  val;    /* Value for SETVAL */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short  *array;  /* Array for GETALL, SETALL */
   struct seminfo  *__buf;  /* Buffer for IPC_INF
                              (Linux-specific) */
};

int broker_createsem(char *key_file, int key_id, int flags, int sem_size);
void *broker_createbuffer(char *key_file, int key_id, 
                          int flags, int shm_size, int *shmid);
int broker_sendmsg(int semid, long src, long dst, long count, long offset, 
          long opcode, long result, char *name, char *data);
int broker_rcvmsg(int semid);

int broker_lock(int semid);
int broker_unlock(int semid);
int broker_initsem(int semid);
void initialize_node(struct broker_shm_info broker_buff_info,
                     struct broker_shm_info node_buff_info,
                     struct broker_shm_info node_info_buff_info );

int register_node(struct node_info);
