#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/sysinfo.h"

int main(int argc, char *argv[]){
    struct sysinfo info;
    sysinfo(&info);
    printf("Number of process: %d\n", info.nproc);
    printf("Free memory: %d\n", info.freemem);
    exit(0);
}