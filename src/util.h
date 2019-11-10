
/*
 * Author: Jose Moran
 * email : jmoran071996@gmail.com 
 * github: jose0796
 * 
 */

#define COMMAND_SIZE 1024
#define TKNR 128
#define MAXP 32
#define RD_END 0
#define WR_END 1

#define IN  0
#define OUT 1
#define IO  2



void        prompt();
int         enumerate_command(char * []);
void        split(char **, char *, const char * );
int         verify(char ** , const char *);
void        file_io_handler(char *,char * argv[], char * inputfilename,char * outputfilename, int in_out);
void        pipe_handler(char * argv[], int );
void        execute(char * );
void        handle_command(char * []);
