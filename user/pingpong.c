#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char* argv[]){
    int p[2];
    char buf[1];
    char *parent = "p";
    char *child = "c";
    pipe(p);
    if(fork() == 0){
        read(p[0], buf, 1);
        printf("child recieved %c\n", buf[0]);
        close(p[0]);

        write(p[1], child, 1);
        close(p[1]);
        exit(0);
    }else{
        write(p[1], parent, 1);
        close(p[1]);
        wait(0);

        read(p[0], buf, 1);
        printf("parent recieved %c\n", buf[0]);
        close(p[0]);
        exit(0);
    }
}