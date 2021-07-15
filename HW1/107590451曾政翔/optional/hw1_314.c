#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[])
{   const int SIZE = 4096;
/* name of the shared memory object */
const char *name = "OS";
/* shared memory file descriptor */
int shm_fd;
/* pointer to shared memory obect */
void *ptr;
/* open the shared memory object */
shm_fd = shm_open(name, O RDONLY, 0666);
/* memory map the shared memory object */
ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
/* write to the shared memory object */
sprintf(ptr,"%s","hello");
ptr += strlen("hello");
sprintf(ptr,"%s","wolrd");
ptr += strlen("wolrd");
/* read from the shared memory object */
printf("%s",(char *)ptr);
/* remove the shared memory object */
shm unlink(name);
    int n = atoi(argv[1]);
    if( n < 0 ){
        printf("no positive integer\n");
        return 0;
    }
    pid_t pid;
    /* fork a child process */

    pid = fork();

    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }else if (pid == 0) { /* child process */
        printf("%d\n",n);
        while (n!=1)
        {
            if(n % 2 == 0){
                n /= 2;
            }else{
                n = n * 3 + 1;
            }
            printf("%d\n",n);
        }
    }else { /* parent process */
        /* parent will wait for the child to complete */
        wait(NULL);
        printf("Child Complete\n");
    }
    return 0;
}