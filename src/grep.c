/*
 * Author: Jose Moran
 * email : jmoran071996@gmail.com 
 * github: jose0796
 * 
 */

#define _GNU_SOURCE 

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "grep.h"


FILE * grep_flag_handler(char * argv[], char * pattern, int * flags){
    
    /* 
        Handles flags given as parameters to grep 

        @param  char * []   argv
        @param  char *      pattern 
        @param  int  *      flags 

     */



    FILE * fp; 
    char filename[255];
    int found_pattern = 0; 
    int found_file = 0; 
    int len; 


    // calculate size of argv
    int size = 0; 
    while(argv[size]!= NULL){
        ++size; 
    }

    if ( size == 1){ // nothing given, throw error 
        return NULL;
    }

    for(int i = 1; i < size && strcmp(argv[i], ">"); ++i){

        if(argv[i][0] != '-'){
            if (!strcmp(argv[i],"<")){ // input file 
                fp = fopen(argv[i+1], "r");
                if (fp == NULL){
                    printf("grep: %s: No such file or directory", argv[i+1]);
                    exit(-1);
                }
            }else if (!found_pattern){ 
                strcpy(pattern, argv[i]);
                found_pattern = 1; 
            }else if (!found_file){
                fp = fopen(argv[i], "r"); 
                found_file = 1;
            }
        }else{

            len = strlen(argv[i]);
            
            for (int j = 1; j < len; ++j){
                switch(argv[i][j]){
                    case 'i':
                        *flags |= IGN; 
                        break; 
                    case 'v':
                        *flags |= INV; 
                        break;
                }
            } 
            

        }
    }


    if (!found_file){
        fp = fdopen(STDIN_FILENO, "r");
    }



    return fp;


}


int main( int argc, char * argv[]){

    FILE * fp; 
    char pattern[100]; 
    int flags = 0; 

    
    fp = grep_flag_handler(argv, pattern, &flags);

    char buffer[255];
    char * s;
    if (fp == NULL){
        printf("Usage: grep [OPTION]... PATTERN [FILE]..\n"); 
        return -1; 

    }

    fgets(buffer,255, fp);
    while(!feof(fp)){
        
        
        if ((flags & IGN) && (flags & INV)){
            
            if((s = strcasestr(buffer,pattern)) == NULL){
                printf("%s", buffer);
            }
        }else if (flags & INV){
            
            if((s = strstr(buffer,pattern) ) == NULL){
                printf("%s", buffer);
            }
        }else if (flags & IGN){            
            
            if((s = strcasestr(buffer,pattern)) != NULL){
                printf("%s", buffer);
            }
        }else{
            if((s = strstr(buffer,pattern)) != NULL){
                printf("%s", buffer);
            }
        }   
        fgets(buffer,255, fp);
    }

    return 0; 
}
