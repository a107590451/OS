#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
int main(int argc, char *argv[])
{	int n = atoi(argv[1]);
	
    if( n < 0 ){
        printf("no positive integer\n");
        return 0;
    }
    /* the size (in bytes) of shared memory object */
	const int SIZE = 4096;
	/* name of the shared memory object */	
	const char *name = "/OS";
	
	
    pid_t pid;	
    pid = fork();

    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }else if (pid == 0) { /* child process */
    	
    	int *ptr;
	int shm_fd;
	/* create the shared memory object */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	/* configure the size of the shared memory object */
	ftruncate(shm_fd, SIZE);
	/* memory map the shared memory object */
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	/* write to the shared memory object */
	
	int i=1;
	ptr[i]=n;	
        while (n!=1)
        {	
        	i++;
            if(n % 2 == 0){
                n /= 2;
            }else{
                n = n * 3 + 1;
            }
            ptr[i]=n;
        }
        ptr[0]=i;
    }else { /* parent process */
        /* parent will wait for the child to complete */
        wait(NULL);
        int *ptr;
	int shm_fd;
	/* create the shared memory object */
	shm_fd = shm_open(name, O_RDONLY, 0666);
	/* memory map the shared memory object */
	ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	//printf("size:%d\n",sizeof(ptr)/sizeof(ptr[0]));
	for(int i=0; i< ptr[0];i++){
		printf("%d\n",ptr[i+1]);
	}
	
	shm_unlink(name);
        printf("process Complete\n");
    }
    return 0;
}