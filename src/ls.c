#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<dirent.h>
#include<sys/types.h>
#include<string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdbool.h>

#define DETAIL  0b1
#define HIDDEN  0b10
#define INDEX   0b100
#define OGRP    0b1000
#define OWNR    0b10000
#define HMN     0b100000
#define RCRS    0b1000000
#define COLOR   0b10000000


#define KB      1024
#define MB      1048576
#define GB      1073741824

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char * human_readable(int size){

    static char s[10];
    float div; 

    if (size < KB){
        sprintf(s, "%6d", size);
    }
    else if (size >= KB && size < MB){
        div = (float) size / (float) KB; 
        sprintf(s, "%5.1fK", div);
    }else if (size >= MB && size < GB){
        div = (float) size / (float) MB; 
        sprintf(s, "%5.1fM", div);
    }else {
        div = (float) size / (float) GB; 
        sprintf(s, "%5.1fG", div);
    }


    return(s);


}


char * time_format(struct timespec m_time){

    struct tm * time_struct; 
    static char str[30];
    time_struct = localtime( (time_t * ) &m_time);
    strftime(str,sizeof(str), "%h %e %H:%M", time_struct);
    return(str);



}



void detailed_printing(char * filename, struct stat fileSt, int flags, int colored){

    struct passwd * pw = getpwuid(fileSt.st_uid); 
    struct group * gr  = getgrgid(fileSt.st_gid); 

    //---------permissions 
    
    printf((S_ISDIR(fileSt.st_mode))? "d": "-"); 
    printf((fileSt.st_mode & S_IRUSR)? "r": "-");
    printf((fileSt.st_mode & S_IWUSR)? "w": "-");
    printf((fileSt.st_mode & S_IXUSR)? "x": "-");
    printf((fileSt.st_mode & S_IRGRP)? "r": "-");
    printf((fileSt.st_mode & S_IWGRP)? "w": "-");
    printf((fileSt.st_mode & S_IXGRP)? "x": "-");
    printf((fileSt.st_mode & S_IROTH)? "r": "-");
    printf((fileSt.st_mode & S_IWOTH)? "w": "-");
    printf((fileSt.st_mode & S_IXOTH)? "x": "-");
    printf(" %1d", (int) fileSt.st_nlink);
    if (!(flags & OWNR )){
        printf( " %2s", pw->pw_name); 
    }

    if (!(flags & OGRP )){
        printf( " %2s" , gr->gr_name); 
    }
    
    if (flags & HMN){
        printf(" %s", human_readable((int) fileSt.st_size));
    }else{
        printf(" %10d", (int) fileSt.st_size); 
    }
    
    printf(" %s" ,time_format(fileSt.st_mtim)); 
    if (COLOR)
        printf( ANSI_COLOR_CYAN " %s \n" ANSI_COLOR_RESET, filename); 
    else 
        printf( " %s \n" , filename); 


}


void printdir(char * dir, int flags){

    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    char directories[100][100];
    int num_subds = 0 ; 
    if((dp = opendir(dir)) == NULL) {
        if ((dp = opendir(getenv("PWD"))) == NULL){
            printf("Error opening directory.");
            
        }else{
            while((entry = readdir(dp)) != NULL){
                if (!strcmp(entry->d_name, dir)){
                    if (flags & INDEX){
                        printf("%2d ", (int )  statbuf.st_ino); 
                    }
                    if (flags & DETAIL){
                        detailed_printing(entry->d_name, statbuf, flags, 0); 
                    }else{
                        printf("%s  ",entry->d_name);
                    }
                }

            }
            printf("ls: cannot access '%s': No such a file or directory\n", dir);
        }
        return;
    }
    chdir(dir); // change directory 
    while((entry = readdir(dp)) != NULL) {
        //printf("\n%s\n, flags = %d", entry->d_name, flags);

        lstat(entry->d_name,&statbuf);
        if((strcmp(".",entry->d_name) == 0 ||
            strcmp("..",entry->d_name) == 0 || 
            (entry->d_name[0] == '.')) && !(flags & HIDDEN)) 
            continue;

        if(S_ISDIR(statbuf.st_mode)) {
            /* Found a directory, but ignore . and .. */ 
            if (flags & INDEX){
                printf("%2d ", (int )  statbuf.st_ino); 
            }
            if (flags & DETAIL){
                detailed_printing(entry->d_name, statbuf, flags, COLOR); 
            }else{
                printf( ANSI_COLOR_CYAN " %s " ANSI_COLOR_RESET,entry->d_name);
            }
            if (flags & RCRS && flags & DETAIL){
                strcpy(directories[num_subds], entry->d_name);
                ++num_subds;

            }

        }
        else {

            if (flags & INDEX){
                printf("%2d ", (int )  statbuf.st_ino); 
            }
            if (flags & DETAIL){
                detailed_printing(entry->d_name, statbuf, flags, 0); 
            }else{
                printf("%s  ",entry->d_name);
            }
        
        }

        
    }
    printf("\n");
    flags = flags ^ RCRS;
    for (int x = 0; x < num_subds; ++x){
        printf("./%s: \n\n", directories[x]); 
        printdir(directories[x], flags);
    }
    
    chdir("..");
    closedir(dp);

}

int num_par(char * argv[]){
    int i = 0; 
    while(argv[i]!= NULL){
        ++i; 
    }
    return i;
}



int flagParser(char * argv[], char * path){

    int num = num_par(argv);
    int len = 0; 
    int flags = 0; 
    int found_path = 0;

    for(int i = 1 ; i < num; ++i){

        len = strlen(argv[i]);

        if (argv[i][0] != '-'){
            strcpy(path, argv[i]); 
            found_path = 1;
            return flags;
        }

        for(int j = 0; j < len; ++j){
            if (argv[i][j] != '-') {
                switch(argv[i][j]){

                    case 'l': 
                        flags = flags | DETAIL; 
                        break;
                    
                    case 'i':
                        flags = flags | INDEX; 
                        break; 
                    
                    case 'a':
                        flags = flags | HIDDEN; 
                        break;

                    case 'G':
                        flags = flags | OGRP; 
                        break;
                    
                    case 'g':
                        flags = flags | OWNR; 
                        break;
                    
                    case 'h':
                        flags = flags | HMN; 
                        break;
                    
                    case 'R':
                        flags = flags | RCRS; 
                        break;
                    
                    default: 
                        break;

                }
            }
        }


        // if (!strcmp(argv[i], "-l")){
        //     flags = flags & DETAIL;
        // }
        // else if (!strcmp(argv[i], "-a") ||
        //          !strcmp(argv[i], "-la") ||
        //          !strcmp(argv[i], "-ia") || 
        //          !strcmp(argv[i], "-Ga")){
        //     flags = flags & HIDDEN; 
        // }else if (!strcmp(argv[i], "-i")){
        //     flags = flags & INDEX; 
        // }else if (!strcmp(argv[i], "-G")){
        //     flags = flags & OGRP; 
        // }else if (!strcmp(argv[i], "-g")){
        //     flags = flags & OWNR; 
        // }else if (!strcmp(argv[i], "-h")){
        //     flags = flags & HMN; 
        // }else if (!strcmp(argv[i], "-R")){
        //     flags = flags & RCRS; 
        // }else if (!strcmp(argv[i], "-la"))


    }
    if (!found_path){
        strcpy(path, getenv("PWD"));
    }


    
    return flags; 


}


int main(int argc, char * argv[]){

    struct stat fileSt; 
    struct dirent dir; 

    //printf("argv[1] = %s", argv[1]);

    if (argv[1] == NULL){
        printdir(getenv("PWD"), 0);
        
    }else if (argv[1][0] != '-'){
        printdir(argv[1], 0);
    }else{
        char path[1024]; 
        int flags = flagParser(argv, path);
        printdir(path, flags);
    }




    //if (lstat(argv[1],&fileSt) < 0 ) return -1; 

    
    
    


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




    return 0;
}