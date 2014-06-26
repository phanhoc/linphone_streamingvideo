#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf-2.0/gdk-pixbuf/gdk-pixbuf-core.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <string.h>
#include <time.h> 


#define WIDTH 600
#define HEIGHT 600
#define PORT 8000
#define MAXMSG 1024


GtkWidget *image;
GtkWidget *window;
GtkWidget *text;
GtkBuilder *gtkBuilder;
char location[100];
char recvBuff[MAXMSG];
FILE *output_file;
GdkPixbuf *pixbuf;

void loadImage(char *path);
int socket_init();
int connect_server(char *ip, int sockfd);
void send_request(int sockfd);
void hp_send_clicked(GtkWidget *button);