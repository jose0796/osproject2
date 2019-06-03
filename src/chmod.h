

#define RD  0b1
#define WR  0b10
#define XC  0b100
#define NRD 0b1000
#define NWR 0b10000
#define NXC 0b100000


void    change_permission(char * filename, int flags);      // change permissions of given file 
int     chmod_size(char * args[]);                                // number of arguments 
int     chmod_flag_parser(char * argv[], char * filename);  // returns flags based on parameters given
int     mychmod(char * argc[]);                             // main function 

