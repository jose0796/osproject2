

#define COMMAND_SIZE 1024
#define TKNR 128
#define MAXP 32
#define RD_END 0
#define WR_END 1

#define IN  0
#define OUT 1
#define IO  2

//char cwd[1024];
// pid_t tesh_pgid; 
// int tesh_terminal, tesh_is_interactive; 
// struct termios tesh_tmodes; 
typedef int(*pfOperator)(char **);


//void        init_tesh();
void        prompt();
int         enumerate_command(char * []);
void        split(char **, char *, const char * );
int         verify(char ** , const char *);
pfOperator  function_selector(char * command); 
void        file_io_handler(char * argv[], char * inputfilename,char * outputfilename, int in_out);
void        pipe_handler(char * argv[], int );
void        execute(char * );
// int         find_char(char * []);
void        handle_command(char * []);
