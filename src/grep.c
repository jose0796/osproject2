#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>


int main(int args, char * argv[]){

    FILE * fp = fdopen(STDIN_FILENO,"r"); 

    char buffer[255];
    char * s;

    while(!feof(fp)){
        fgets(buffer,255, fp);
        if(!((s = strstr(buffer,argv[1]) )== NULL)){
            printf("%s", buffer);
            
        }
    }



    return 0; 
}
