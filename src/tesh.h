

#define CSIZE 256
#define TKNR 128
#define MAXP 32
#define RD_END 0
#define WR_END 1

#define IN  0
#define OUT 1
#define IO  2

//void        init_tesh();


int         enumerate_command(char * []);
int         separar(char **, char *, char * );
int         existe(char ** , const char *);
void        file_io_handler(char * argv[], char * inputfilename,char * outputfilename, int in_out);
// void        (char * argv[], int );
void        ejecutar(char * );
void        manejadorDeComandos(char *);
