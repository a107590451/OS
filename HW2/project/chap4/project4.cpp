#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include<algorithm>
#include <iostream>

using namespace std;
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

int sudoku[9][9] =
    {
            {6, 2, 4, 5, 3, 9, 1, 8, 7},
            {5, 1, 9, 7, 2, 8, 6, 3, 4},
            {8, 3, 7, 6, 1, 4, 2, 9, 5},
            {1, 4, 3, 8, 6, 5, 7, 2, 9},
            {9, 5, 8, 2, 4, 7, 3, 6, 1},
            {7, 6, 2, 3, 9, 1, 4, 5, 8},
            {3, 7, 1, 9, 5, 6, 8, 4, 2},
            {4, 9, 6, 1, 8, 2, 5, 7, 3},
            {2, 8, 5, 4, 7, 3, 9, 1, 6}
     };
vector<int> grid[9];    
int gridnum=0;
int validValue = 1;
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
void* calrow(void* voidA) {
    int pid;
    vector <int> row(9);
    pthread_mutex_lock(&mutex);
    printf("row lock\n"); 
    
     pid = bithead->pid;
    printf("pid = %d\n", pid);
    bithead->bitmap=1;
    bithead = bithead->next;
    
   int valid =1;
    for(int i=0;i<9;i++){
    	for(int j=0;j<9;j++){
    		row[j] = sudoku[i][j];	
    		 //cout<<"debug"<<endl;
    	}
    	//cout<<"debug"<<endl;
    	sort(row.begin(),row.end());
    	for(int j=0;j<9;j++){
    		if(row[j] != j+1){
    			cout<<"sukofu is invalid"<<endl;
    			cout<<"row:"<<j<<"j:"<<j+1<<endl;
    			valid =0;
    			validValue = 0;
    			break;
    		}		 
    	}
    	if(valid == 0)
    		break;
    }
    
    if(valid == 1)
	cout<<"sukofu is valid"<<endl;
    if(bithead->next==NULL){
        printf("error:no space for thread\n");
        while(1);
    }
   
    release_pid(pid);
    //printf("debug\n");
    pthread_mutex_unlock(&mutex);
    printf("row unlock\n"); 
    pthread_exit(NULL);
}
void* calcol(void* voidA) {
    int pid;
    vector <int> col(9);
    pthread_mutex_lock(&mutex);
    printf("collock\n");
     
    pid = bithead->pid;
    printf("pid = %d\n", pid);
    bithead->bitmap=1;
    bithead = bithead->next;
    
   int valid =1;
    for(int i=0;i<9;i++){
    	for(int j=0;j<9;j++){
    		col[j] = sudoku[j][i];	
    		 //cout<<"debug"<<endl;
    	}
    	//cout<<"debug"<<endl;
    	sort(col.begin(),col.end());
    	for(int j=0;j<9;j++){
    		if(col[j] != j+1){
    			cout<<"sukofu is invalid"<<endl;
    			cout<<"row:"<<j<<"j:"<<j+1<<endl;
    			valid =0;
    			validValue = 0;
    			break;
    		}		 
    	}
    	if(valid == 0)
    		break;
    }
    
    if(valid == 1)
	cout<<"sukofu is valid"<<endl;
    if(bithead->next==NULL){
        printf("error:no space for thread\n");
        while(1);
    }
    
    release_pid(pid);
    //printf("debug\n");
    pthread_mutex_unlock(&mutex);
    printf("col unlock\n"); 
    pthread_exit(NULL);
}
void* calgrid(void* voidA) {

	pthread_mutex_lock(&mutex);
	printf("grid lock\n");
	
	int pid = bithead->pid;
    	printf("pid = %d\n", pid); 
    	bithead->bitmap=1;
    	bithead = bithead->next;
	int num = gridnum;
	gridnum++;
	
	cout<<"gridnum:"<<num<<endl;
	vector <int> row(9);
    
    
	int valid =1;
   
    	sort(grid[num].begin(),grid[num].end());
    	vector<int>  :: iterator iter = grid[num].begin();
    	for(int ix = 1; iter != grid[num].end(); ++iter, ++ix){
    		
    		if(*iter!= ix){
    			cout<<"sukofu is invalid"<<endl;
    			//cout<<"row:"<<j<<"j:"<<j+1<<endl;
    			valid =0;
    			validValue = 0;
    			break;
    		}		 
    	}
    
    
    if(valid == 1)
	cout<<"sukofu is valid"<<endl;
    if(bithead->next==NULL){
        printf("error:no space for thread\n");
        while(1);
    }
    
    release_pid(pid);
    //printf("debug\n");
    pthread_mutex_unlock(&mutex);
     printf("grid unlock\n"); 
     pthread_exit(NULL);
}
int main() 
{   
	
     
    pthread_t thread[100];
   
    int map = allocate_map();//初始化bitmapTable，若成功回傳1
    if (map == 1) {
        printf("\nData Structure initialized.\n");
        
        pthread_mutex_init(&mutex, NULL);
        pthread_create(&thread[0], NULL, calrow, NULL);
        
        pthread_mutex_init(&mutex, NULL);
        pthread_create(&thread[1], NULL, calcol, NULL);
        
        
        for(int i= 0;i<9;i++){
        	for(int j=0;j<9;j++){
        		grid[i].push_back(sudoku[j/3+(i/3)*3][j%3+(i%3)*3]);
        		//cout<<"grid["<<i<<"]:"<<grid[j]<<endl;
        	}
        	 pthread_mutex_init(&mutex, NULL);
        	 pthread_create(&thread[i+2], NULL, calgrid, NULL);
        	 
        	 printf("thread %d: \n", i);
        }
        
        pthread_join(thread[0], NULL);
        printf("thread :0 released.\n");
        pthread_join(thread[1], NULL);
        printf("thread :1 released.\n");
        	
        for(int i = 2; i < 11; i++){
        	pthread_join(thread[i], NULL);
        	printf("thread :%d released.\n",i);
        	pthread_mutex_destroy(&mutex);
        	
    	}
        if(validValue == 0)
        	cout<<"The sudoku is invalid"<<endl;
        else
        	cout<<"The sudoku is valid"<<endl;
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


