#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<dirent.h>
#include<sys/types.h>
#include<string.h>


void printdir(char * dir, int depth){

    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(dir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
    chdir(dir); // change directory 
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            /* Found a directory, but ignore . and .. */
            if(strcmp(".",entry->d_name) == 0 ||
                strcmp("..",entry->d_name) == 0)
                continue;
            printf("%*s%s/\n",depth,"",entry->d_name);
            /* Recurse at a new indent level */
            //printdir(entry->d_name,depth);
        }
        else printf("%*s%s\n",depth,"",entry->d_name);
    }
    chdir("..");
    closedir(dp);

}


int main(int argc, char * argv[]){

    struct stat fileSt; 
    struct dirent dir; 


    //if (lstat(argv[1],&fileSt) < 0 ) return -1; 

    char str[100] = ""; 

    printdir(getenv("PWD"), 0);


    // sprintf(str, (S_ISDIR(fileSt.st_mode))? "d": "-"); 
    // strcat(str, (fileSt.st_mode & S_IRUSR)? "r": "-");
    // strcat(str, (fileSt.st_mode & S_IWUSR)? "w": "-");
    // strcat(str, (fileSt.st_mode & S_IXUSR)? "x": "-");
    // strcat(str, (fileSt.st_mode & S_IRGRP)? "r": "-");
    // strcat(str, (fileSt.st_mode & S_IWGRP)? "w": "-");
    // strcat(str, (fileSt.st_mode & S_IXGRP)? "x": "-");
    // strcat(str, (fileSt.st_mode & S_IROTH)? "r": "-");
    // strcat(str, (fileSt.st_mode & S_IWOTH)? "w": "-");
    // strcat(str, (fileSt.st_mode & S_IXOTH)? "x": "-");




    printf("%s\n", str);



    return 0;
}