#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
sem_t accessible;
// 子執行緒函數
void *northFarmer(void *arg) {//call northFarmer 
	sem_wait(&accessible);
	int walkTime = rand()%3+1;
   	printf("northFarmer enter walkTime:%ds\n",walkTime);
  	sleep(walkTime);
   	sem_post(&accessible);
   	printf("northFarmer exit\n");
   	pthread_exit(NULL);
}
void *southFarmer(void *arg) {//call southFarmer 
	sem_wait(&accessible);
	int walkTime = rand()%3+1;
   	printf("southFarmer enter walkTime:%ds\n",walkTime);
  	sleep(walkTime);
   	sem_post(&accessible);
   	printf("southFarmer exit\n");
	pthread_exit(NULL);
}
// 主程式
int main() {
   //init
   srand(time(NULL));
   pthread_t northFarmers[5];
   pthread_t southFarmers[5];
   sem_init(&accessible, 0, 1);
   

   // 建立子執行緒
   for(int i=0;i<5;i++){
   
   	pthread_create(&northFarmers[i], NULL, northFarmer, NULL);
   	pthread_create(&southFarmers[i], NULL, southFarmer, NULL);
   }
   // 等待子執行緒完畢
   for(int i=0;i<5;i++){
   	pthread_join(southFarmers[i], NULL);
   	printf("south pthread[%d] end\n",i);
   	pthread_join(northFarmers[i], NULL);
   	printf("north pthread[%d] end\n",i);
   }
   sem_destroy(&accessible);

   return 0;
}