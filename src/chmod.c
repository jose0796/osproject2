#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<string.h>

#define RD  0b1
#define WR  0b10
#define XC  0b100
#define NRD 0b1000
#define NWR 0b10000
#define NXC 0b100000


void change_permission(char * filename, int flags){
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


int size(char * args[]){
    int i = 0; 
    while(args[i] != NULL){
        ++i; 
    }
    return i; 

}


#define FLAG 1

int flags_parser(char *  args[], char * filename){

    int flags = 0; 

    int len = strlen(args[1]);

    if (args[FLAG] != NULL){

        if (args[FLAG][0] == '+'){

            for(int i = 1; i < len; ++i){
                if (args[FLAG][i] == 'r'){
                    flags = flags | RD; 
                }else if (args[FLAG][i] == 'w'){
                    flags = flags | WR; 
                }else if (args[FLAG][i] ==  'x'){
                    flags = flags | XC; 
                }
            }
        }else if (args[FLAG][0] == '-'){
            for(int i = 1; i < len; ++i){
                if (args[FLAG][i] == 'r'){
                    flags = flags | NRD; 
                }else if (args[FLAG][i] == 'w'){
                    flags = flags | NWR; 
                }else if (args[FLAG][i] ==  'x'){
                    flags = flags | NXC; 
                }
            }
        }else{
            strcpy(filename, args[FLAG]);
        }

    }
    if (args[2] != NULL){
        strcpy(filename, args[2]);
    }
    
    
    
    return flags; 

}




int main(int args, char * argv[]){

    char filename[100]; 
    int flags = flags_parser(argv, filename);

    // printf("filename: %s\n", filename); 
    // printf("flags: %d\n", flags);
    if (filename == NULL){
        printf("chmod: missing operand\n");
        return -1;
    }
    else if (!flags){
        printf("chmod: missing operand after '%s'\n", filename);
        return -1;
    }else{
        change_permission(filename, flags);

    }

	
}
