#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<dirent.h>
#include<sys/types.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include "ls.h"

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



void detailed_printing(char * filename, struct stat fileSt, int flags){

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
        printf( " %4s", pw->pw_name); 
    }

    if (!(flags & OGRP )){
        printf( " %4s" , gr->gr_name); 
    }
    
    if (flags & HMN){
        printf(" %s", human_readable((int) fileSt.st_size));
    }else{
        printf(" %10d", (int) fileSt.st_size); 
    }
    printf(" %s" ,time_format( fileSt.st_mtim)); 
    printf( " %s \n" , filename); 


}


void printdir(char * dir, int flags){

    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    int error; 

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
                        lstat(entry->d_name, &statbuf);
                        detailed_printing(entry->d_name, statbuf, flags); 
                        
                    }else{
                        printf("%s ",entry->d_name);
                    }
                    printf("\n");
                    return;   
                }

            }
            printf("ls: cannot access '%s': No such a file or directory\n", dir);
        }
        
        return;
    }
    chdir(dir); // change directory 
    while((entry = readdir(dp)) != NULL) {
        

        error = lstat(entry->d_name,&statbuf);
        if (error < 0){
            printf("ls: cannot access '%s': No such a file or directory\n", dir); 
            break; 
        }
       
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
                detailed_printing(entry->d_name, statbuf, flags); 
            }else{
                printf( "%s  ",entry->d_name);
            }
            if (flags & RCRS){
                strcpy(directories[num_subds], entry->d_name);
                ++num_subds;

            }

        }
        else {

            if (flags & INDEX){
                printf("%2d ", (int )  statbuf.st_ino); 
            }
            if (flags & DETAIL){
                detailed_printing(entry->d_name, statbuf, flags); 
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

int ls_size(char * argv[]){
    int i = 0; 
    while(argv[i]!= NULL){
        ++i; 
    }
    return i;
}



int ls_flag_parser(char * argv[], char * path){

    int num = ls_size(argv);
    int length = 0; 
    int len = 0; 
    int flags = 0; 
    int found_path = 0;
    char buffer[100];

    for(int i = 1 ; i < num && strcmp(argv[i],">"); ++i){

        

        if (argv[i][0] != '-' && !found_path){
            
            if (argv[i][0] == '<'){ // input file 
                FILE * fp = fopen(argv[i+1], "r"); 
                
                if (fp == NULL){
                    printf("tesh: %s: No such a file or directory.", argv[i+1]);
                    return -1;
                }

                fgets(buffer,100,fp);
                length = strlen(buffer);
                buffer[length-1] = '\0';
                strcpy(path,buffer);
                fclose(fp);

            }else{ // just use given directory 
                strcpy(path, argv[i]);              
            }
            found_path = 1; 
        }else if (argv[i][0] == '-'){

            len = strlen(argv[i]);

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

        
        }



    }
    if (!found_path){
        strcpy(path, getenv("PWD"));
    }


    
    return flags; 


}


int myls(char * argv[]){

    struct stat fileSt; 
    struct dirent dir; 

    if (argv[1] == NULL){
        printdir(getenv("PWD"), 0);
        
    }else if (argv[1][0] != '-' && argv[1][0] != '<' && argv[1][0] != '>'){
        printdir(argv[1], 0);
    }else{
        char path[1024]; 
        int flags = ls_flag_parser(argv, path);
        printdir(path, flags);
    }

    return 0;
}

