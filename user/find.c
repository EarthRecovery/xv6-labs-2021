#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void findpath(char* path, char* filename){
    char buf[512], *p;

    int fd;
    struct dirent de;
    struct stat st;

    fd = open(path, 0);
    fstat(fd, &st);
    read(fd, &de, sizeof(de));

    switch(st.type){
        case T_FILE:
            if (strcmp(fmtname(path), filename) == 0){
                printf("%s\n", path);
            }
            break;
        case T_DIR:
            strcpy(buf,path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if((de.inum == 0) || (strcmp(de.name, ".") == 0) || (strcmp(de.name, "..") == 0)){
                    continue;
                }
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                // 获得文件描述
                if(stat(buf, &st) < 0){
                    fprintf(2, "find: cannot stat %s\n", buf);
                    continue;
                }

                if (st.type == T_FILE){
                    if (strcmp(de.name, filename) == 0){
                        printf("%s\n", buf);
                    }
                }else if (st.type == T_DIR){
                    // 如果当前目录是文件夹, 则递归处理, buf为下一级目录
                    findpath(buf, filename);
                }
            }
            break;
    }
}

int main(int argc, char* argv[]){
    char *path = argv[1];
    char *filename = argv[2];
    findpath(path, filename);
    exit(0);
}