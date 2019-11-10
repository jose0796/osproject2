/*
 * Author: Jose Moran
 * email : jmoran071996@gmail.com 
 * github: jose0796
 * 
 */

#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<string.h>
#include "chmod.h"


void change_permission(char * filename, int flags){
    /* 
        Changes permissions of file
        @param  char *   filename
        @param  int      flags 
        @return void 

     */


    struct stat fileSt; 
    lstat(filename, &fileSt);

    int mode = fileSt.st_mode; 

    if (flags & RD){
        mode = mode | S_IRUSR | S_IRGRP | S_IROTH; 
    }
    if (flags & WR){
        mode = mode | S_IWUSR | S_IWGRP ; 
    }
    if (flags & XC){
        mode = mode | S_IXUSR | S_IXGRP | S_IXOTH; 
    }
    if (flags & NRD){
        mode = mode & ~S_IRUSR & ~S_IRGRP & ~S_IROTH; 
    }
    if (flags & NWR){
        mode = mode & ~S_IWUSR & ~S_IWGRP & ~S_IWOTH; 
    }
    if (flags & NXC){
        mode = mode & ~S_IXUSR & ~S_IXGRP & ~S_IXOTH; 
    }

    chmod(filename, mode); 
}


int chmod_size(char * args[]){ 
    int i = 0; 
    while(args[i] != NULL){
        ++i; 
    }
    return i; 

}




int chmod_flags_parser(char *  args[], char * filename){

    /* 
        Parser for flags received 
        @param  char *[]    args
        @param  char *      filename
        @return int         flags 
     */



    int flags = 0; 
    int siz = chmod_size(args);
    int len = 0; 

    for (int i = 0; i < siz && strcmp(args[i], ">"); ++i){

        len = strlen(args[i]); // 

        if (args[i][0] == '+'){ // add perm. 

            for(int j = 1; j < len; ++j){ 
                if (args[i][j] == 'r'){ //-> read permission
                    flags = flags | RD; 
                }else if (args[i][j] == 'w'){ // -> write permission 
                    flags = flags | WR; 
                }else if (args[i][j] ==  'x'){ // -> execution permission 
                    flags = flags | XC; 
                }
            }
        }else if (args[i][0] == '-'){ // remove perm.
            for(int j = 1; j < len; ++j){
                if (args[i][j] == 'r'){
                    flags = flags | NRD; 
                }else if (args[i][j] == 'w'){
                    flags = flags | NWR; 
                }else if (args[i][j] ==  'x'){
                    flags = flags | NXC; 
                }
            }
        }else if (args[i][0] == '<'){
            strcpy(filename, args[i]);
        }
    }

    return flags; 

}




int main(int argc, char * argv[]){

    char filename[100]; 
    int flags = chmod_flags_parser(argv, filename);

    
    if (filename == NULL){ // nothing was given 
        printf("chmod: missing operand\n");
        return -1;
    }
    else if (!flags){ // incorrent flags 
        printf("chmod: missing operand after '%s'\n", filename);
        return -1;
    }else{
        change_permission(filename, flags);
    }
    return 0;

	
}
