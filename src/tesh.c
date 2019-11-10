
/*
 * Author: Jose Moran
 * email : jmoran071996@gmail.com 
 * github: jose0796
 * 
 */


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


int main(){
    setenv("PATH", "/home/sam/Documents/Github/tesh/bin", 1); 
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

    /* 
        Command Prompt Info 
     */
    char hname[1024] = "";
    char path[256]; 
    gethostname(hname, sizeof(hname));
    getcwd(path,256); 
    printf("%s@%s:%s$--> ", getenv("USER"), hname, path);
}

int enumerate_command(char * args[]){

    /* 
        Determine number of pipe separated commands 
     */

    int i = 0;
    while(args[i]!= NULL){
        ++i; 
    }
    return i; 
}

void split(char * __dst[], char * __src, const char * s){

    /* 
        Splits a string into tokens 

        @param char *   __dst[]  final tokens 
        @param char *   __src    string to parse 
        @param char *   s        splitting character
    
     */

    int i = 0;
    if ((__dst[i] = strtok(__src, s)) != NULL){
        do{
            ++i;
        }while((__dst[i] = strtok(NULL,s)) != NULL); 
    }

}

int verify(char ** args, const char * s){

    // verify character existence 

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

void find_path(char * path, char * cmd){

    char cpath[255] = ""; 
    char * tks[20];
    split(tks,getenv("PATH"), ":"); 
    int i = 0; 
    while(tks[i] != NULL){
        
        strcat(cpath,tks[i]); 
        strcat(cpath,"/"); 
        strcat(cpath, cmd); 
        if (access(cpath, F_OK) != -1){
            strcpy(path, cpath);
            return; 
        }
            
        ++i; 
        
    } 
    path = ""; 
    printf("tesh: command not found\n");
    
    
}



void execute(char * arg){

    int i = 0; 
    int in = -1; 
    int out = -1; 
    char path[255];
    
    pid_t child; 
    char * command[MAXP]; 
    split(command, arg, " "); 
    if ( command[0]  != NULL ){

        child = fork(); 
        if (child == 0){

            signal(SIGINT, SIG_IGN);
                        
            find_path(path, command[0]); 
            if (strcmp(path,"")){
                in = verify(command, "<"); 
                out = verify(command, ">");

                if (in < 0 && out < 0){
                    execv(path, command);
                }else if ( in < 0 ){
                    file_io_handler(path, command, NULL, command[out], OUT);
                }else if (out < 0){
                    file_io_handler(path,command, command[in], NULL, IN);
                }else {
                    file_io_handler(path,command, command[in], command[out], IO);
                }

            }else{
                printf("tesh: command not found\n");
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

void file_io_handler(char * path, char * command[], char * inputfilename, char * outputfilename, int in_out){

    int fd; 
    FILE * fp; 
    

        
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

    execv(path,command);

    
    
}


void pipe_handler(char * args[], int num_cmds){


    /* 
        Pipe handler process piped commands sequences

        A sequence of commands separated by pipes may be handle as 
        follows: 
                     __output ___
                    |            |
            cmd1   |   cmd2     |    cmd3
                  |            | 
          input---------------- 

        2 pipes are needed to handle in-between pipes commands. 

        @param char * args[]    pipe separated commands
        @param int    num_cmds  number of commands received
    
     */

    
    char * command[MAXP];
    
    int pps[2][2]; // 2 pipes 
    pid_t child_pid; 
    
    int i = 0, j = 0; 
    int in = -1, out = -1; 
    char path[256];


    pipe(pps[0]); 
    pipe(pps[1]);

    while( args[i] != NULL && i < num_cmds){
        
            
        child_pid = fork(); 

        if (child_pid < 0){

            // ERROR on CREATION
            if (i < num_cmds-1){
                close(pps[i%2][WR_END]);   
            }
            printf("Child process creation failed.");
            return; 


        }else if (child_pid == 0){

            // this is child process 


            split(command,args[i]," ");  // splits single command 

            find_path(path,command[0]);
            if (i == 0){

                // duplicate stdout file descriptor to pipe write end
                dup2(pps[0][WR_END], STDOUT_FILENO); 


                in = verify(command, "<"); // is there any "<" ? s

                if (in > 0){ // handle redirection 
                    file_io_handler(path,command,command[in],NULL,IN);
                }else{
                    execv(path,command) ;
                    
                    
                }

            }

            else if ( i == num_cmds-1){
                dup2(pps[(i+1)%2][RD_END], STDIN_FILENO);
                out = verify(command, ">");
                if (out > 0){
                    file_io_handler(path,command,NULL,command[out],OUT);
                }else{
                    execv(path,command);
                    
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

                execv(path,command);
                
   
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

    /* 
        Handles Commands entered from main interface

        It splits command based on space characteres and 
        process built in commands. Otherwise it uses either
        pipe_handler (for piped command sequence) or execute
        for single non built-in commands.

        @param char *  []   args
    
     */

    char * command[256];
    char tmp[32];

    strcpy(tmp,args[0]);

    split(command, tmp, " "); 


    
    int nro_cmd = enumerate_command(args); 

    // process built-in 
    if (!strcmp(command[0], "exit")){
        exit(0);
    }
    else if (!strcmp(args[0], "clear")){
        system("clear");
    }
    else if (!strcmp(args[0], "pwd")){
        char buff[256]; 
        getcwd(buff,255);
        printf("%s\n", buff);

    }else if(!strcmp(command[0],"cd")){
        chdir(command[1]);

    }
    
    else{ 

        if (nro_cmd > 1){
            pipe_handler(args, nro_cmd); 

        }else{
            execute(args[0]); 
        }


    }

}


