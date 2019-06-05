

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<fcntl.h>
#include "tesh.h"
#include "grep.h"
#include "ls.h"
#include "chmod.h"

int main(int args , char *argv[]){

    char cmd[CSIZE]; 
    
    char str[255];

    if (argv[1] != NULL){ 
        // FILE * fp = fopen(argv[1], "r");
        // if (fp == NULL){
        //     perror("tesh: No such a file or directory");
        // }else{

        //     int size = 0; 
        //     while(!feof(fp)){
        //         fgets(str,255,fp);
        //         size++;
        //     }


        //     rewind(fp);

        //     for (int i = 0; i < size-1; ++i){
        //         cmd = 0;
        //         fgets(command, CSIZE, fp);
        //         if ((cmds[cmd] = strtok(command,"|\n") )!= NULL){
        //             ++cmd; 
        //             while((cmds[cmd] = strtok(NULL, "|\n")) != NULL && cmd< TKNR) ++cmd; 
                    
        //             handle_command(cmds);
        //         }
                
        //     }
            
        // }

    }else { // ---------interactive 

        while(1){
            printf("$ ");
            fgets(cmd, CSIZE, stdin);
            if (strcmp(cmd,"\n")){
                manejadorDeComandos(cmd);
            }
        }



    }

    
    

}






int separar(char ** a, char * b, char * s){

    int i = 1;

    a[0] = strtok(b, s);
    if (a[0] != NULL){
        while((a[i] = strtok(NULL,s)) != NULL) ++i;
    }else{
        i = 0; 
    }
    return i; 

}

int existe(char ** s, const char * c){

    int i = 0; 

    

    while( s[i] != NULL){
        if(!strcmp(s[i], c)){
            return i+1;
        }
        ++i; 
    }
    return -1;


}




// pfOperator function_selector(char * command){

//     if (!strcmp(command, "ls")){
//         return(myls); 
//     }else if (!strcmp(command, "grep")){
//         return (mygrep);
//     }else if (!strcmp(command,"chmod")){
//         return (mychmod);
//     }else {
//         return NULL;
//     }


// }

// void execute(char * arg){

//     int i = 0; 
//     int in = -1; 
//     int out = -1; 

//     pid_t child; 
//     char * command[MAXP]; 
//     split(command, arg, " "); 
//     if ( command[0]  != NULL ){

//         child = fork(); 
//         if (child == 0){

//             signal(SIGINT, SIG_IGN);
//             func = function_selector(command[0]);

//             if (func != NULL) {
//                 in = verify(command, "<"); 
//                 out = verify(command, ">");

//                 if (in < 0 && out < 0){
//                     (*func)(command);
//                 }else if ( in < 0 ){
//                     file_io_handler(command, NULL, command[out], OUT);
//                 }else if (out < 0){
//                     file_io_handler(command, command[in], NULL, IN);
//                 }else {
//                     file_io_handler(command, command[in], command[out], IO);
//                 }

//             }

//             exit(0);


//         }else{
//             waitpid(child,NULL,0);
//         }

//     }else{
//         perror("Command not found."); 
//         return; 
//     }



// }

// void file_io_handler(char * command[], char * inputfilename, char * outputfilename, int in_out){

//     int fd; 
//     FILE * fp; 
//     int (*func)(char **); 

//     func = function_selector(command[0]);
   
        
//     if (in_out == OUT){ // output redirection to file
//         fp = fopen(outputfilename, "w"); 
//         fd = fileno(fp);
//         dup2(fd, STDOUT_FILENO);
//         close(fd);
//     }else if (in_out == IN){
//         fp = fopen(inputfilename, "r"); 
//         fd = fileno(fp);
//         dup2(fd, STDIN_FILENO);
//         close(fd);
//     }else if (in_out == IO){
//         fp = fopen(inputfilename, "r"); 
//         fd = fileno(fp);
//         dup2(fd, STDIN_FILENO);
//         close(fd);

//         fp = fopen(outputfilename, "w"); 
//         fd = fileno(fp);
//         dup2(fd, STDOUT_FILENO);
//         close(fd);

//     }

//     func(command);
    
// }


// void pipe_handler(char * args[], int num_cmds){

//     char * command[MAXP];
//     int pps[2][2]; 
//     pid_t child_pid; 
//     int i = 0, j = 0; 
//     int in = -1, out = -1; 
//     int (*func)(char **); 

//     pipe(pps[0]);
//     pipe(pps[1]);
//     while( args[i] != NULL && i < num_cmds){
        
            
//         child_pid = fork(); 

//         if (child_pid < 0){
//             if (i < num_cmds-1){
//                 close(pps[i%2][WR_END]); 
                
//             }
//             printf("Child process creation failed.");
//             return; 
//         }else if (child_pid == 0){
//             split(command,args[i]," "); 
//             func = function_selector(command[0]);
//             if (i == 0){
//                 dup2(pps[0][WR_END], STDOUT_FILENO);

//                 in = verify(command, "<"); 
//                 if (in > 0){
//                     file_io_handler(command,command[in],NULL,IN);
//                 }else{
//                     (*func)(command);
//                 }

//             }

//             else if ( i == num_cmds-1){
//                 dup2(pps[(i+1)%2][RD_END], STDIN_FILENO);
//                 out = verify(command, ">");
//                 if (out > 0){
//                     file_io_handler(command,NULL,command[out],OUT);
//                 }else{
//                     (*func)(command);
//                 }

//             }
//             else{
//                 if (i % 2 != 0){
//                     dup2(pps[1][WR_END], STDOUT_FILENO); 
//                     dup2(pps[0][RD_END], STDIN_FILENO); 
//                 }else {
//                     dup2(pps[1][RD_END], STDIN_FILENO); 
//                     dup2(pps[0][WR_END], STDOUT_FILENO);
//                 }

                
//                 (*func)(command);
   
//             }
           
//             exit(0);
            
            
//         }else{

//             if (i == 0){
//                 close(pps[0][WR_END]); 
            
//             }
            
//             else if (i == num_cmds - 1) {
//                 close(pps[(i+1)%2][RD_END]);
//             }else {
                
//                 if (i % 2 != 0 ){
//                     close(pps[0][RD_END]);
//                     close(pps[1][WR_END]);
//                 }else{

//                     close(pps[1][RD_END]);
//                     close(pps[0][WR_END]);
//                 }
                 
//             }

//             waitpid(child_pid, NULL,0);
//             ++i; 
            


//         }


//     }


// }

void exc(char * name, char * param[]){
    if (!strcmp(name, "ls")){
        myls(param);
    }else if (!strcmp(name, "grep")){
        mygrep(param);
    }else if (!strcmp(name, "chmod")){
        mychmod(param);
    }else{
        printf("tesh: comando no encontrado.");
    }
}

void manejadorDeComandos(char * command){

    char * cmds[256];
    FILE * infp; 
    FILE * outfp;
    pid_t  child;  
    int infd, outfd; 
    int nro_comandos = separar(cmds,command,"\n|");


    if (!strcmp(cmds[0],"exit")){
        exit(0);
    }else{

        if (nro_comandos  == 1){

            

            child = fork();

            if (child == 0){

                char * cmd[32];
                int tam = separar(cmd, cmds[0], " ");
                int entrada = existe(cmd,"<");
                int salida  = existe(cmd,">");
                


                if (entrada > 0){
                    if (cmd[entrada] == NULL){
                        printf("error: indique el nombre de un archivo");
                        exit(100);
                    }else{
                        infp = fopen(cmd[entrada], "r");
                        if (infp == NULL){
                            printf("error: el archivo de entrada indicado no existe.");
                            exit(101);
                        }else{
                            infd = fileno(infp);
                            dup2(infd,STDIN_FILENO);
                            close(infd);
                        }
                    }
                }

                if (salida > 0){
                    if (cmd[salida] == NULL){
                        printf("error: indique el nombre de un archivo");
                        exit(100);
                    }else{
                        outfp = fopen(cmd[salida], "w");
                        if (outfp == NULL){
                            printf("error: el archivo de salida indicado no existe.");
                            exit(102);
                        }else{
                            outfd = fileno(outfp);
                            dup2(outfd,STDIN_FILENO);
                            close(outfd);
                        }
                    }
                }

                exc(cmd[0], cmd);
                exit(0);
            }else if (child > 0){

                waitpid(child,NULL,0);

            }


        }else if (nro_comandos > 1){


            for(int i = 0; i < nro_comandos; ++i){


                




            }



        }




    }









    // char tmp[32];

    // strcpy(tmp,args[0]);

    // split(command, tmp, " "); 

    // int nro_cmd = enumerate_command(args); 
    // if (!strcmp(command[0], "exit")){
    //     exit(0);
    // }
    // else if (!strcmp(args[0], "clear")){
    //     system("clear");
    // }
    // else if (!strcmp(args[0], "pwd")){
    //     printf("%s\n", getenv("PWD"));
    // }else{

    //     if (nro_cmd > 1){
    //         pipe_handler(args, nro_cmd); 

    //     }else{
    //         execute(args[0]); 
    //     }


    // }

}


