
#include "util.h"

int main(){
    char * tokens[TKNR]; 
    int tks = 0; 
    char command[COMMAND_SIZE]; 

    init_tesh();

    while(1){
        tks = 0;
        prompt();
        memset ( command, '\0', COMMAND_SIZE );
        fgets(command, COMMAND_SIZE, stdin);
        if ((tokens[tks] = strtok(command,"|\n") )!= NULL){
            ++tks; 
            while((tokens[tks] = strtok(NULL, "|\n")) != NULL && tks< TKNR) ++tks; 
            // for(int i = 0; i  < tks; ++i){
            //     printf("%s\n", tokens[i]);
            // }
            handle_command(tokens);
        }

    }

}

void init_tesh(){

    /* See if we are running interactively.  */
  tesh_terminal = STDIN_FILENO;
  tesh_is_interactive = isatty (tesh_terminal);

  if (tesh_is_interactive)
    {
      /* Loop until we are in the foreground.  */
      while (tcgetpgrp (tesh_terminal) != (tesh_pgid = getpgrp ()))
        kill (- tesh_pgid, SIGTTIN);

      /* Ignore interactive and job-control signals.  */
      signal (SIGINT, SIG_IGN);
      //signal (SIGQUIT, SIG_IGN);
      signal (SIGTSTP, SIG_IGN);
      signal (SIGTTIN, SIG_IGN);
      signal (SIGTTOU, SIG_IGN);
      signal (SIGCHLD, SIG_IGN);

      /* Put ourselves in our own process group.  */
      tesh_pgid = getpid ();
      if (setpgid (tesh_pgid, tesh_pgid) < 0)
        {
          perror ("Couldn't put the shell in its own process group");
          exit (1);
        }

      /* Grab control of the terminal.  */
      tcsetpgrp (tesh_terminal, tesh_pgid);

      /* Save default terminal attributes for shell.  */
      tcgetattr (tesh_terminal, &tesh_tmodes);
    }





}


void prompt(){
    char hname[1024] = "";
    gethostname(hname, sizeof(hname));
    printf("%s@%s:%s $--> ", getenv("USER"), hname, getenv("PWD"));
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
        }while((__dst[i] = strtok(NULL,s)) != NULL && i < MAXP); 
    }



}

void execute(char * arg){

    int i = 0; 
    pid_t child; 
    char * command[MAXP]; 
    split(command, arg, " "); 
    if ( command[0]  != NULL ){

        child = fork(); 
        if (child == 0){

            signal(SIGINT, SIG_IGN);
            setenv("PATH", "/home/sam/Documents/Operativos/osproject2/bin/", 1);
            printf("%s\n", command[0]);
            if (execvp(command[0], command) < 0){
                printf("Command not found.\n");
                kill(getpid(), SIGTERM);
            }


        }else{
            waitpid(child,NULL,0);
        }

    }else{
        perror("Command not found."); 
        return; 
    }



}

void pipe_handler(char * args[], int num_cmds){

    char * command[MAXP];
    int pps[2][2]; 
    int fd[2]; 
    int fd2[2];
    pid_t child_pid; 
    int i = 0; 
    int j = 0; 


    while( args[i] != NULL && i < num_cmds){
        pipe(pps[i%2]);

        child_pid = fork(); 

        if (child_pid < 0){
            if (i < num_cmds-1){
                close(pps[i%2][WR_END]); 
                
            }
            printf("Child process creation failed.");
            return; 
        }else if (child_pid == 0){
            if (i == 0){
                dup2(pps[0][WR_END], STDOUT_FILENO);
            }

            else if ( i == num_cmds-1){
                dup2(pps[(i+1)%2][RD_END], STDIN_FILENO);
                
            }
            else{
                if (i % 2 != 0){
                    dup2(pps[1][WR_END], STDOUT_FILENO); 
                    dup2(pps[0][RD_END], STDIN_FILENO); 


                }else {
                    dup2(pps[1][RD_END], STDIN_FILENO); 
                    dup2(pps[0][WR_END], STDOUT_FILENO);
                }
                
            }
            setenv("PATH", "/home/sam/Documents/Operativos/osproject2/bin/", 1);
            split(command,args[i]," "); 
            if (execvp(command[0], command) < 0){
                perror("Could not initiate child process.\n");
                kill(getpid(), SIGTERM);
            }
            
        }else{

            if (i == 0){
                close(pps[0][WR_END]); 
            }else if (i == num_cmds - 1)
            {
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

    int nro_cmd = enumerate_command(args); 
    if (!strcmp(args[0], "exit")){
        exit(0);
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


