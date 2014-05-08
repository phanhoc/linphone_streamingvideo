#include <sys/socket.h>
#include <sys/types.h>

#include <ifaddrs.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <netdb.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include <pthread.h>

#define PORT 5555
#define MAXMSG 1024

#define MSG1 "REQUEST"
#define MSG2 "RESPONSE" 
#define MSG3 "OUT"

/*Hien thi ma loi va noi dung*/
void error(char *msg);
/*Lay thoi gian hien tai*/
char* now();
/*Ghi vao file*/
void write_file(char *file_name,char *msg);
/*Check ip*/
int check_ip(char *file_name,char *ip);
/*Lay clock den microsecond*/
const char* clock_microsecond();
/*Get timezone*/
static int get_timezone(void);
/*Get file size*/
int get_file_size(FILE *fd);
/*Get file name*/
char* get_file_name(char* path_to_file_name);