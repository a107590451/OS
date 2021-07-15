#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <vector>
#include <unistd.h>
#include <algorithm>
using namespace std;
sem_t accessible;
sem_t indexlock;
pthread_t northFarmers[5];
typedef struct
{
	int A;
	int B;
	int C;
} processResource;
typedef struct
{
	processResource allocation;
	processResource max;
} process;

processResource bank;
vector<process> processList;
int customerindex =0;
bool safe(vector<process> processList,int eraseEntry,processResource bank){
	
	if(eraseEntry != -1){
		processList.erase(processList.begin()+eraseEntry);
	}
	if(processList.size()==0){
		return true;
	}
	vector<process>::iterator it ;
	for(it = processList.begin();it != processList.end();it++){
		if(bank.A > (it->max.A - it->allocation.A) && bank.B > (it->max.B - it->allocation.B) && bank.C > (it->max.C -it->allocation.C)){//available>need 
			bank.A += it->allocation.A;
			bank.B += it->allocation.B;
			bank.C += it->allocation.C;
			return safe(processList,it - processList.begin(),bank);
		}			
	}
	if(it == processList.end()){
		return false;
	}
	
}

// 子執行緒函數
void *customer(void *arg) {//call northFarmer 
	process mypro;
	processResource mybank;
	vector<process> myprocessList;
	sem_wait(&indexlock);
	int index = customerindex; 
	customerindex++;
	sem_post(&indexlock);
	myprocessList.assign(processList.begin(),processList.end());
	int inittime;
	
	mypro.max.A = rand() % bank.A;
	mypro.max.B = rand() % bank.B;
	mypro.max.C = rand() % bank.C;
	if(mypro.max.A != 0){
		mypro.allocation.A = rand() % mypro.max.A;
	}else{
		mypro.allocation.A = 0;
	}
	if(mypro.max.B != 0){
		mypro.allocation.B= rand() % mypro.max.B;
	}else{
		mypro.allocation.B = 0;
	}
	if(mypro.max.C != 0){
		mypro.allocation.C = rand() % mypro.max.C;
	}else{
		mypro.allocation.C = 0;
	}
	
	
	while(1){
		sem_wait(&accessible);
		mybank.A =bank.A;
		mybank.B =bank.B;
		mybank.C =bank.C;
		myprocessList.push_back(mypro);
		if(safe(myprocessList,-1,mybank)){//success entry CPU
			processList.push_back(mypro);
		
			inittime = rand()%3+1;
			printf("inittime:%ds\n",inittime);
			printf("max.A:%d max.B:%d max.C:%d\n",mypro.max.A,mypro.max.B,mypro.max.C);
			printf("allocation.A:%d allocation.B:%d allocation.C:%d\n",mypro.allocation.A,mypro.allocation.B,mypro.allocation.C);
			bank.A -= mypro.allocation.A;
			bank.B -= mypro.allocation.B;
			bank.C -= mypro.allocation.C;
			printf("Avaliable.A:%d Avaliable.B:%d Avaliable.C:%d\n",bank.A,bank.B,bank.C);
			sem_post(&accessible);
			sleep(inittime);
			
			while(1){
				sem_wait(&accessible);
				if(bank.A > (mypro.max.A - mypro.allocation.A) && bank.B > (mypro.max.B - mypro.allocation.B) && bank.C > (mypro.max.C -mypro.allocation.C)){//available>need 
					bank.A += mypro.allocation.A;
					bank.B += mypro.allocation.B;
					bank.C += mypro.allocation.C;
					vector<process>::iterator it ;
					for(it = processList.begin();it != processList.end();it++){
						if(it->max.A == mypro.max.A && it->max.B == mypro.max.B && it->max.C == mypro.max.C && it->allocation.A == mypro.allocation.A && it->allocation.B == mypro.allocation.B 
						&& it->allocation.C == mypro.allocation.C){
							processList.erase(it);
							printf("customer erase\n");
							break;
						}
						
					}
					
					
					printf("customer[%d] end\n",index);
					printf("Avaliable.A:%d Avaliable.B:%d Avaliable.C:%d\n",bank.A,bank.B,bank.C);
					break;	
				}
				sem_post(&accessible);
			}
				
		}
		sem_post(&accessible);
	}
	
}

// 主程式
int main(int argc, char *argv[]) {
   //init
   if(argc != 4){
   		printf("error input !");
   		return 0;
   }
   //init bank
   bank.A = atoi(argv[1]);
   bank.B = atoi(argv[2]);
   bank.C = atoi(argv[3]);
   printf("bank :\nA=%d B=%d C=%d\n",bank.A,bank.B,bank.C);
   
   srand(time(NULL));
   pthread_t processor[5];

   sem_init(&accessible, 0, 1);
   sem_init(&indexlock, 0, 1);

   // 建立子執行緒
   for(int i=0;i<3;i++){
   
   	pthread_create(&processor[i], NULL, customer, NULL);
   	printf("pthread[%d] creat\n",i);

   }
   // 等待子執行緒完畢
   for(int i=0;i<3;i++){
   	pthread_join(processor[i], NULL);
   	printf("pthread[%d] end\n",i);
   }
   sem_destroy(&accessible);
   sem_destroy(&indexlock);
   processList.clear();

   return 0;
}