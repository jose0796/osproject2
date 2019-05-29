

#include<termios.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>

#define COMMAND_SIZE 1024
#define TKNR 128
#define MAXP 32
#define RD_END 0
#define WR_END 1


pid_t tesh_pgid; 
int tesh_terminal, tesh_is_interactive; 
struct termios tesh_tmodes; 


void init_tesh();
void prompt();
int enumerate_command(char * []);
void pipe_handler(char * [], int );
void __execute(char * );
int find_char(char * []);
void handle_command(char * []);