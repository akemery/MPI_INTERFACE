/* Definitions needed by clients and servers */

#define TRUE           1
#define MAX_PATH     255  /* maximum length of file name*/
#define BUF_SIZE    1024  /* how much data to transfer at once */
#define FILE_SERVER  243 /* file server's network address */

/* Definitions of the allowed operations*/
#define CREATE         1 /* create a new file */
#define  READ          2 /* read data from a file and return it*/ 
#define WRITE          3 /* write data to a file */
#define DELETE         4 /* delete an existing file */
#define STOP           5 /* Stop transfert*/

/* error codes*/
#define OK 0  /*operation performed correctly*/
#define E_BAD_OPCODE  -1 /* unknown oepration requested */
#define E_BAD_PARAM   -2 /* error in parameter */
#define E_IO          -3  /* disk error or other I/O error*/ 

/* Definition of the message format*/

struct message{
  long source;          /* senders identity */
  long dest;            /* receiver's identity */
  long opcode;          /* requested operation */
  long count;           /* number of byte to transfer */
  long offset;          /* position in file to start I/O */
  long result;          /* result of the operation*/
  char name[MAX_PATH];  /* name of file being operated on*/
  char data[BUF_SIZE];  /* data to read or written*/
};

/* Definition of broker function*/

int receive_m(int src, struct message *m);
int send_m(int dst, struct message *m);

/* Definition of server functions*/
long do_create(struct message *m1, struct message *m2);
long do_read(struct message *m1, struct message *m2);
long do_write(struct message *m1, struct message *m2);
long do_delete(struct message *m1, struct message *m2);

/* Definition of client functions*/

void initialize_client(void);
void initialize_server(void);
