#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int *fd){
    close(fd[1]);
    int p,d;
    read(fd[0], &p, sizeof(int));
    printf("prime %d\n", p);
    if (read(fd[0], &d, sizeof(d))){
        int fd1[2];
        pipe(fd1);
        if (fork() == 0){
            primes(fd1);
        }else{
            // 关闭读
            close(fd1[0]);
            do{
                if (d % p != 0){
                    write(fd1[1], (void *)&d, sizeof(d));
                }
            }while(read(fd[0], (void *)&d, sizeof(d)));
            // 关闭读
            close(fd[0]);
            // 关闭写
            close(fd1[1]);
            wait(0);
        }
    }

}

int main(int argc, char* argv[]){
    int fd[2];
    int start = atoi(argv[1]);
    int end = atoi(argv[2]);
    pipe(fd);
    if(fork() == 0){
        primes(fd);
    }else{
        close(fd[0]);
        for(int i=start; i<=end; i++){
            write(fd[1], &i, sizeof(int));
        }
        close(fd[1]);
        wait(0);
    }

    exit(0);
}