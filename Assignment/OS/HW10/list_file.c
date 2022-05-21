#include <stdio.h>     // fprintf, perror
#include <stdlib.h>    // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>    // strlen, strncpy, strncmp, strncat
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>    // getopt
#include <dirent.h>    // opendir, readdir, closedir
#include <time.h>      // strftime, localtime
#include <pwd.h>
#include <grp.h>

void print_permission(struct stat sb){
    char permission[] = "----------";

    if(S_ISBLK(sb.st_mode))         permission[0] = 'b';
    else if(S_ISCHR(sb.st_mode))    permission[0] = 'c';
    else if(S_ISDIR(sb.st_mode))    permission[0] = 'd';
    else if(S_ISLNK(sb.st_mode))    permission[0] = 'l';

    if(sb.st_mode & S_IRUSR)        permission[1] = 'r';
    if(sb.st_mode & S_IWUSR)        permission[2] = 'w';
    if(sb.st_mode & S_IXUSR)        permission[3] = 'x';
    
    if(sb.st_mode & S_IRGRP)        permission[4] = 'r';
    if(sb.st_mode & S_IWGRP)        permission[5] = 'w';
    if(sb.st_mode & S_IXGRP)        permission[6] = 'x';
    
    if(sb.st_mode & S_IROTH)        permission[7] = 'r';
    if(sb.st_mode & S_IWOTH)        permission[8] = 'w';
    if(sb.st_mode & S_IXOTH)        permission[9] = 'x';

    printf("%s  ", permission);
}

void print_file(struct stat sb) {
    uid_t user = sb.st_uid;
    gid_t group = sb.st_gid;
    struct passwd *psw = getpwuid(user);
    struct group *grp = getgrgid(group);
    // struct group *grp;
    print_permission(sb);
    printf("%3ld  ", (long) sb.st_nlink);
    printf("%3s  %3s    ", psw->pw_name, grp->gr_name);
    printf("%4lld   ", (long long) sb.st_size);
    char timeString[1024] = "";
    strftime(timeString, 1024, "%b %d %H:%M", localtime(&sb.st_mtime));
    printf("%s  ", timeString);
}

int main(int argc, char **argv){
    /* without "-l" program should print the file names 
     * with "-l" program should print the information about each file 
    */
    int opt, l_opt = 0;
    char *pathname = ".";
    if(argc == 2 && strcmp(argv[1], "-l") == 0)     l_opt = 1;
    else{
        while((opt = getopt(argc, argv, "l:")) != -1){
        switch(opt){
            case 'l':
                pathname = optarg;
                l_opt = 1;
                break;
            case '?':
                if (optopt == 'l')  l_opt = 1;
                break;
            default:    
                break;
        }
    }
    }
    

    /* if withoout "-l" option, setting pathname */
    if(l_opt == 0 && argc == 2)  pathname = argv[1];

    struct stat sb;
    if(lstat(pathname, &sb) == -1){
        perror("dirpath stat call failed!\n");
        exit(1);
    }

    /* print file informatin in directory */
    if(S_ISDIR(sb.st_mode)){
        DIR *dir_info;
        if((dir_info = opendir(pathname)) == NULL){
            perror("opendir call failed!\n");
            exit(1);
        }
        struct dirent *dir_entry;
        while((dir_entry = readdir(dir_info)) != NULL){
            if(l_opt == 1){
                char filePath[1024] = "";
                strncpy(filePath, pathname, strlen(pathname));
                strncat(filePath, "/", 1);
                strncat(filePath, dir_entry->d_name, strlen(dir_entry->d_name));
                if (stat(filePath, &sb) == -1){
                    perror("filepath stat call failed!\n");
                    exit(1);
                }
                print_file(sb);
            }
            printf("%s\n", dir_entry->d_name);
        }
        closedir(dir_info);
    }
    else{
        if(l_opt == 1)  print_file(sb);
        printf("%s\n", pathname);
    }

    return 0;
}
