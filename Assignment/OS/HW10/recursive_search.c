#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

void print_path(char *pathname, struct stat sb){
    if(S_ISDIR(sb.st_mode)){
        DIR *dir_info;
        if((dir_info = opendir(pathname)) == NULL){
            printf("errno: %d\n",errno);
            perror("opendir call failed!\n");
            exit(1);
        }
        struct dirent *dir_entry;
        while((dir_entry = readdir(dir_info)) != NULL){
            /* Print */
            if(pathname[strlen(pathname)-1] == '/')     printf("%s%s\n", pathname, dir_entry->d_name); 
            else        printf("%s/%s\n", pathname, dir_entry->d_name);

            if(strcmp(dir_entry->d_name, "..") == 0)        continue;
            else if(strcmp(dir_entry->d_name, ".") == 0)    continue;
            else{
                /* make filePath */
                char filePath[1024] = "";
                strncpy(filePath, pathname, strlen(pathname));
                if(filePath[strlen(filePath)-1] != '/')     strncat(filePath, "/", 1);
                strncat(filePath, dir_entry->d_name, strlen(dir_entry->d_name));

                /* recursive */
                struct stat temp;
                if(stat(filePath, &temp) == -1){
                    perror("filepath stat call failed!\n");
                    exit(1);
                }
                print_path(filePath, temp);
            }
        }
        closedir(dir_info);
    }
    else    return;
}

int main(int argc, char **argv){
    char pathname[2048];
    getcwd(pathname, PATH_MAX);
    if(argc == 2){
        strcpy(pathname, argv[1]);
    }

    struct stat sb;
    if(stat(pathname, &sb) == -1){
        perror("dirpath stat call failed!\n");
        exit(1);
    }
    print_path(pathname, sb);

    return 0;
}
