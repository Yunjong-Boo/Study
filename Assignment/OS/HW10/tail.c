#include <stdio.h>     // fprintf, perror
#include <stdlib.h>    // exit, atoi
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>    // lseek, read
#include <fcntl.h>     // open

int main(int argc, char **argv){
    if(argc != 3 || argv[1][0] != '-'){
        perror("command line format is ./mytail [-n] [file]\n");
        exit(1);
    }

    /* setting num_line */
    int num_line = atoi(argv[1]);
    num_line *= -1;
    char *pathname = "";
    pathname = argv[2];

    struct stat sb;
    if((stat(pathname, &sb)) == -1){
        perror("stat call failed!\n");
        exit(1);
    }

    /* open file */
    int fd;
    if((fd = open(pathname, O_RDONLY)) == -1){
        perror("open call failed!\n");
        exit(1);
    }

    /* locate cur to last of the file */
    if(lseek(fd, -1, SEEK_END) == -1){
        perror("lseek call failed, cur is not located last!\n");
        exit(1);
    }

    /* control cur to needed location */
    int cur;
    char buf[sb.st_size];
    while(num_line > 0){
        if(read(fd, buf, 1) == -1){
            perror("read call failed!\n");
            exit(1);
        }
        if(buf[0] == '\n')  num_line--;
        cur = lseek(fd, -2, SEEK_CUR);
        if(cur == -1)   break;
    }

    if(cur == -1)   lseek(fd, 0, SEEK_SET);
    else            lseek(fd, 2, SEEK_CUR);

    /* read num_lines line & print */
    memset(buf, 0, sb.st_size);
    if(read(fd, buf, sb.st_size) == -1){
        perror("read lines failed!\n");
        exit(1);
    }
    printf("%s\n", buf);

    close(fd);

    return 0;
}
