#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<string.h>

#define RD  0x1
#define WR  0x10
#define XC  0x100
#define NRD 0x1000
#define NWR 0x10000
#define NXC 0x100000



int size(char * args[]){
    int i = 0; 
    while(args[i] != NULL){
        ++i; 
    }
    return i; 

}


int flags_parser(char *  args[], char * filename){

    int flags = 0; 

    int num_args = size(args);
    for(int i = 0; i < num_args; ++i){

        if (!strcmp(args[i], "+r")){
            flags = flags | RD; 
        }else if (!strcmp(args[i], "-r")){
            flags = flags | NRD; 
        }else if (!strcmp(args[i], "+w")){
            flags = flags | WR; 
        }else if (!strcmp(args[i], "-w")){
            flags = flags | NWR; 
        }else if (!strcmp(args[i], "+x")){
            flags = flags | XC; 
        }else if (!strcmp(args[i], "-x")){
            flags = flags | NXC; 
        }else{
            strcpy(filename, args[i]); 
        }

    }

    return flags; 

}




int main(int args, char * argv[]){

    char filename[100]; 
    int flags = flags_parser(argv, filename);

    printf("filename: %s\n", filename); 
    printf("flags: %d", flags);


	
}
