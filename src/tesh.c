

#include<termios.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<fcntl.h>

#include "util.h"
#include "grep.h"
#include "ls.h"
#include "chmod.h"

int main(){
    char * tokens[TKNR]; 
    int tks = 0; 
    char command[COMMAND_SIZE]; 

    while(1){
        tks = 0;
        prompt();
        memset ( command, '\0', COMMAND_SIZE );
        fgets(command, COMMAND_SIZE, stdin);
        if ((tokens[tks] = strtok(command,"|\n") )!= NULL){
            ++tks; 
            while((tokens[tks] = strtok(NULL, "|\n")) != NULL && tks< TKNR) ++tks; 
            handle_command(tokens);
        }

    }

}





void prompt(){
    char hname[1024] = "";
    gethostname(hname, sizeof(hname));
    printf("%s@%s:$--> ", getenv("USER"), hname);
}

int enumerate_command(char * args[]){

    int i = 0;
    while(args[i]!= NULL){
        ++i; 
    }
    return i; 
}

void split(char * __dst[], char * __src, const char * s){

    int i = 0;
    if ((__dst[i] = strtok(__src, s)) != NULL){
        do{
            ++i;
        }while((__dst[i] = strtok(NULL,s)) != NULL); 
    }

}

int verify(char ** args, const char * s){

    int i = 0; 
    while( args[i] != NULL){
        if(!strcmp(args[i], s)){
            break; 
        }
        ++i; 
    }
    if (args[i] == NULL){
        return -1; 
    }


    return i+1; 


}




pfOperator function_selector(char * command){

    if (!strcmp(command, "ls")){
        return(myls); 
    }else if (!strcmp(command, "grep")){
        return (mygrep);
    }else if (!strcmp(command,"chmod")){
        return (mychmod);
    }else {
        return NULL;
    }


}

void execute(char * arg){

    int i = 0; 
    int in = -1; 
    int out = -1; 

    int (*func)(char **); 
    pid_t child; 
    char * command[MAXP]; 
    split(command, arg, " "); 
    if ( command[0]  != NULL ){

        child = fork(); 
        if (child == 0){

            signal(SIGINT, SIG_IGN);
            func = function_selector(command[0]);

            if (func != NULL) {
                in = verify(command, "<"); 
                out = verify(command, ">");

                if (in < 0 && out < 0){
                    (*func)(command);
                }else if ( in < 0 ){
                    file_io_handler(command, NULL, command[out], OUT);
                }else if (out < 0){
                    file_io_handler(command, command[in], NULL, IN);
                }else {
                    file_io_handler(command, command[in], command[out], IO);
                }

            }

            exit(0);


        }else{
            waitpid(child,NULL,0);
        }

    }else{
        perror("Command not found."); 
        return; 
    }



}

void file_io_handler(char * command[], char * inputfilename, char * outputfilename, int in_out){

    int fd; 
    FILE * fp; 
    int (*func)(char **); 

    func = function_selector(command[0]);
   
        
    if (in_out == OUT){ // output redirection to file
        fp = fopen(outputfilename, "w"); 
        fd = fileno(fp);
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }else if (in_out == IN){
        fp = fopen(inputfilename, "r"); 
        fd = fileno(fp);
        dup2(fd, STDIN_FILENO);
        close(fd);
    }else if (in_out == IO){
        fp = fopen(inputfilename, "r"); 
        fd = fileno(fp);
        dup2(fd, STDIN_FILENO);
        close(fd);

        fp = fopen(outputfilename, "w"); 
        fd = fileno(fp);
        dup2(fd, STDOUT_FILENO);
        close(fd);

    }

    func(command);
    
}


void pipe_handler(char * args[], int num_cmds){

    char * command[MAXP];
    int pps[2][2]; 
    pid_t child_pid; 
    int i = 0, j = 0; 
    int in = -1, out = -1; 
    int (*func)(char **); 

    pipe(pps[0]);
    pipe(pps[1]);
    while( args[i] != NULL && i < num_cmds){
        
            
        child_pid = fork(); 

        if (child_pid < 0){
            if (i < num_cmds-1){
                close(pps[i%2][WR_END]); 
                
            }
            printf("Child process creation failed.");
            return; 
        }else if (child_pid == 0){
            split(command,args[i]," "); 
            func = function_selector(command[0]);
            if (i == 0){
                dup2(pps[0][WR_END], STDOUT_FILENO);

                in = verify(command, "<"); 
                if (in > 0){
                    file_io_handler(command,command[in],NULL,IN);
                }else{
                    (*func)(command);
                }

            }

            else if ( i == num_cmds-1){
                dup2(pps[(i+1)%2][RD_END], STDIN_FILENO);
                out = verify(command, ">");
                if (out > 0){
                    file_io_handler(command,NULL,command[out],OUT);
                }else{
                    (*func)(command);
                }

            }
            else{
                if (i % 2 != 0){
                    dup2(pps[1][WR_END], STDOUT_FILENO); 
                    dup2(pps[0][RD_END], STDIN_FILENO); 
                }else {
                    dup2(pps[1][RD_END], STDIN_FILENO); 
                    dup2(pps[0][WR_END], STDOUT_FILENO);
                }

                
                (*func)(command);
   
            }
           
            exit(0);
            
            
        }else{

            if (i == 0){
                close(pps[0][WR_END]); 
            
            }
            
            else if (i == num_cmds - 1) {
                close(pps[(i+1)%2][RD_END]);
            }else {
                
                if (i % 2 != 0 ){
                    close(pps[0][RD_END]);
                    close(pps[1][WR_END]);
                }else{

                    close(pps[1][RD_END]);
                    close(pps[0][WR_END]);
                }
                 
            }

            waitpid(child_pid, NULL,0);
            ++i; 
            


        }


    }


}

void handle_command(char * args[]){

    char * command[256];
    char tmp[32];

    strcpy(tmp,args[0]);

    split(command, tmp, " "); 

    int nro_cmd = enumerate_command(args); 
    if (!strcmp(command[0], "exit")){
        exit(0);
    }
    else if (!strcmp(args[0], "clear")){
        system("clear");
    }
    else if (!strcmp(args[0], "pwd")){
        printf("%s\n", getenv("PWD"));
    }else{

        if (nro_cmd > 1){
            pipe_handler(args, nro_cmd); 

        }else{
            execute(args[0]); 
        }


    }

}


