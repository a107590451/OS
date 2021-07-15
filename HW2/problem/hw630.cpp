#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>


#define MIN_PID 300
#define MAX_PID 5000
#define cb CHAR_BIT

typedef struct bittable
{
    bool bitmap;
    int pid;
    struct bittable *next;
    struct bittable *prev;
}bittable;

int sz = MAX_PID - MIN_PID + 1;
bittable Table[4700];
bittable *bithead;
bittable *bithtail;

//static bittable *Table;
unsigned char *b;
int threadNum = 101;
int allocate_map();

pthread_mutex_t mutex;
/* Releases a pid */
void release_pid(int pid) {
    if (pid < 300) {
        printf("\nInvalid PID: It should lie between 500 and 3000.");
        return;
    }
    Table[pid-300].bitmap = 0;
    bittable *next,*prev;
    next = Table[pid-300].next;
    prev = Table[pid-300].prev;
    
    if(prev!=NULL){
  
    	next->prev = prev;
    	prev->next = next;
    	bithtail->next = &Table[pid-300];
    	Table[pid-300].prev = bithtail;
    	Table[pid-300].next = NULL;
    	bithtail = &Table[pid-300];
    }else{
    	next->prev = NULL;
    	
    	bithtail->next = &Table[pid-300];
    	Table[pid-300].prev = bithtail;
    	Table[pid-300].next = NULL;
    	bithtail = &Table[pid-300];
    }
    printf("release PID: %d\n",pid);
}
void* allocate_pid(void* voidA) {
    int pid;
    pthread_mutex_lock( &mutex ); // 上鎖
    printf("lock\n");
    

    /* 產生 [0, 1) 的浮點數亂數 */
    double x = (double) rand() *100/ (RAND_MAX + 1.0);

    if(bithead->next==NULL){
        printf("error:no space for thread\n");
        while(1);
    }
    pid = bithead->pid;
    printf("pid = %d\n", pid);
    printf("sleep = %.2f\n", x);
    bithead->bitmap=1;
    bithead = bithead->next;
    
   
    //pthread_exit(NULL);
    //printf("debug\n");
    usleep(x);
    //printf("debug\n");
    release_pid(pid);
    //printf("debug\n");
    pthread_mutex_unlock( &mutex ); // 解鎖
     printf("unlock\n");
    //return pid;
}
int main() 
{   
    pthread_t thread[100];
    srand( time(NULL) );
    int map = allocate_map();//初始化bitmapTable，若成功回傳1
    if (map == 1) {
        printf("\nData Structure initialized.\n");
        for(int i= 0;i<101;i++){
        	pthread_mutex_init(&mutex, NULL);
        	 pthread_create(&thread[i], NULL, allocate_pid, NULL);
        	 printf("thread %d: \n", i);
        }
        for(int i = 0; i < 101; i++){
        	pthread_join(thread[i], NULL);
        	printf("thread :%d released.\n",i);
        	
    	}
        pthread_mutex_destroy(&mutex);
    }
    else{
        printf("\nFailed to initialize data structure.\n");
    } 
        
}

/* Creates and initializes a data structure for representing pids;
 returns —1 if unsuccessful, 1 if successful */
int allocate_map() {
    int i;
    for(i=0 ; i < sz+1 ; i++){
        Table[i].bitmap=0;
        Table[i].pid = i+300;
        if(i==0){
            Table[i].next = &Table[i+1];
             Table[i].prev = NULL;
        }else if(i==sz){
            Table[i].next = NULL;
            Table[i].prev = &Table[i-1];
        }else{
            Table[i].next = &Table[i+1];
            Table[i].prev = &Table[i-1];
        }     
    }
    bithead = &Table[0];
    bithtail = &Table[sz];
    if(i==(sz+1))
        return 1;
    return -1;
}

/* Allocates and returns a pid; returns -1
if unable to allocate a pid (all pids are in use) */


