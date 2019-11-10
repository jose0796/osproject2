
/*
 * Author: Jose Moran
 * email : jmoran071996@gmail.com 
 * github: jose0796
 * 
 */

#define DETAIL  0b1
#define HIDDEN  0b10
#define INDEX   0b100
#define OGRP    0b1000
#define OWNR    0b10000
#define HMN     0b100000
#define RCRS    0b1000000
#define COLOR   0b10000000


#define KB      1024
#define MB      1048576
#define GB      1073741824

char *  human_readable(int size);                                       //  human readable size 
char *  time_format(struct timespec m_time);                            //  format time for printing
void    detailed_printing(char * filename, struct stat fileSt, int);    //  printing function for -l flag
void    printdir(char * dir, int flags);                                //  base function 
int     ls_size(char * args[]);                                            //  returns size of parameters list 
int     ls_flag_parser(char ** arg, char * path);                       //  parsing function for parameters 
int     myls(char ** args);                                             //  main function 
