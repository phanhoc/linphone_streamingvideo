#include "sendfile.h"

char *filename;

gboolean update_progress_bar (gpointer data)
{
  gtk_progress_bar_pulse (GTK_PROGRESS_BAR (data));

  /* Return true so the function will be called again; returning false removes
   * this timeout function.
   */
  return TRUE;
}

int main(int argc, char *argv[]){
    GtkBuilder *gtkBuilder;
    GtkWidget *window;
    gtk_init(&argc, &argv);
    
    gtkBuilder = gtk_builder_new();
    gtk_builder_add_from_file(gtkBuilder, "sendfile.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "window"));
    
     /* Connect signals */
    gtk_builder_connect_signals( gtkBuilder, NULL );
    
    GtkWidget *progress_bar = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "progressbar"));
    gtk_progress_bar_pulse (GTK_PROGRESS_BAR (progress_bar));
    gint func_ref = g_timeout_add(50, update_progress_bar, progress_bar);
    
    g_object_unref(G_OBJECT(gtkBuilder));
    gtk_widget_show(window);
    gtk_main();
    g_source_remove (func_ref);
    
    return 0;
}

/*Ham thoat chuong trinh*/
void hp_exit_clicked(GtkWidget *button){
  printf("Nhay vao ham exit\n");
  gtk_main_quit();
}

/*Ham lay ra duong dan toi file truyen*/
void hp_open_clicked(GtkWidget* button)
{
  GtkWidget *dialog;
  filename = (char *)malloc((sizeof(char)));
  dialog = gtk_file_chooser_dialog_new("Open file",NULL,GTK_FILE_CHOOSER_ACTION_OPEN,GTK_STOCK_CANCEL
					  ,GTK_RESPONSE_CANCEL,GTK_STOCK_OPEN,GTK_RESPONSE_ACCEPT,NULL);
  gtk_widget_show(dialog);
  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
  {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
  }
  printf("Duong dan file:%s\n",filename);
  gtk_widget_destroy(dialog);
}

/*Ham gui file di*/
void hp_send_clicked(GtkWidget* button)
{
  int sockfd;
  sockfd = socket_init();
  if(sockfd < 0){
    printf("Loi khong khoi tao duoc socket_init\n");
    //exit(0);
  }
  int temp1 = connect_server("192.168.0.103",sockfd);
  if(temp1 < 0){
    printf("Loi khong connect duoc voi server\n");
    //exit(0);
  }
  send_file(filename, sockfd);
}

/* Ham khoi tao socket
 * Tra ve con so dinh danh cho socket,
 */
int socket_init(){
  int sockfd = 0;  
  sockfd = socket(AF_INET, SOCK_STREAM, 0);	// khoi tao socket
  return sockfd;
}

/* Ham connect den server dang cho nhan file
 * @ip: Dia chi ip cua server,
 * @sockfd: So dinh danh cho socket de truyen
 */
int connect_server(char *ip, int sockfd){
  int res = -1;
  struct sockaddr_in serv_addr;
  memset(&serv_addr, '0', sizeof(serv_addr)); 	// khoi tao chi tiet va setup server muon ket noi 
  serv_addr.sin_family = AF_INET;	// IPv4
  serv_addr.sin_port = htons(PORT); // setup port
  if(inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0){
    printf("\n inet_pton error occured\n");
    return res;
  } 
  
  res = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));	// connect to socket
  return res;
}

/* Ham lay ten file trong duong dan
 * @path_to_file_name: Duong dan den file,
 */
char* get_file_name(char* path_to_file_name ){
  char* file_name;
  file_name = path_to_file_name;
  if(strstr(path_to_file_name,"/")==NULL){
    fprintf(stdout,"Already file name\n");
  }
  else{
    file_name = strstr(path_to_file_name,"/")+1;
    while(strstr(file_name,"/")!=NULL)
      file_name = strstr(file_name,"/")+1;
  }
  return file_name;
}

/* Ham gui file voi 2 tham so dau vao
 * @path_to_file_name: Duong dan den file,
 * @sockfd: So dinh danh cho socket de truyen
 */
void send_file(char *path_to_file_name,int sockfd){
  FILE *fd = fopen(path_to_file_name,"r");
  fseek(fd,0,SEEK_END);
  int size = ftell(fd);
  fseek(fd,0,SEEK_SET);
  if (size%MAXMSG) size = size/MAXMSG + 1;
  else size = size/MAXMSG;
  fprintf(stdout,"Packets : %i\n",size);
  
  char msg[30];
  memset(&msg,'\0', sizeof(msg));
  snprintf(msg,sizeof(msg),"%s",get_file_name(path_to_file_name));
  fprintf(stdout,"%s\n",msg);
  if(send(sockfd,msg,strlen(msg),0)<0) fprintf(stdout,"send name fail");
  usleep(1000);
  
  memset(&msg,'\0', sizeof(msg));
  snprintf(msg,sizeof(msg),"%i",size);
  if (send(sockfd,msg,strlen(msg),0)<0) fprintf(stdout,"send size loi");
  usleep(1000);
  
  int count = 0;
  while(!feof(fd)){
    int wasRead = fread(packet,sizeof(char),MAXMSG,fd);
    send(sockfd,packet,(sizeof(char)*wasRead),0);
    fprintf(stdout,"Count: %i\n",count);
    count++;    
    usleep(1000);
  }
}
