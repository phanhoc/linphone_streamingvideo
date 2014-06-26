#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

#define PORT 5000
#define MAXMSG 1024

char packet[MAXMSG];

void hp_exit_clicked(GtkWidget *button);
void hp_send_clicked(GtkWidget *button);
void hp_open_clicked(GtkWidget *button);

int socket_init();
int connect_server(char *ip, int sockfd);
char *get_file_name(char* path_to_file_name);
void send_file(char *path_to_file_name,int sockfd);