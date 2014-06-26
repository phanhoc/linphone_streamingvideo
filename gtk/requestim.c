#include "requestim.h"

void loadImage(char *path){
  printf("Got image!\n");
  pixbuf = gdk_pixbuf_new_from_file(path,NULL);
  int h = gdk_pixbuf_get_width(pixbuf);
  int w = gdk_pixbuf_get_height(pixbuf);
  pixbuf = gdk_pixbuf_scale_simple(pixbuf,h/2,w/2,GDK_INTERP_BILINEAR);
  gtk_image_set_from_pixbuf(image, pixbuf);
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

void hp_send_clicked(GtkWidget *button){
  printf("send curl\n");  
  strcpy(location,gtk_entry_get_text(GTK_ENTRY(text)));
  strcpy(location,gtk_entry_get_text(text));
  if(strlen(location)==0){
    GtkWidget *dialog;
    gtk_widget_show(window);
    dialog = gtk_message_dialog_new (GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE, "Location is empty. Please enter location!");
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
    return;
  }
  
  int sockfd;
  sockfd = socket_init();
  if(sockfd < 0){
    printf("Loi khong khoi tao duoc socket_init\n");
    //exit(0);
  }
  int temp1 = connect_server("192.168.0.122",sockfd);
  if(temp1 < 0){
    printf("Loi khong connect duoc voi server\n");
    GtkWidget *dialog;
    gtk_widget_show(window);
    dialog = gtk_message_dialog_new (GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE, "Can't connect to server! Please check internet or IP of server.");
    /* Destroy the dialog when the user responds to it (e.g. clicks a button) */
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
    return;
  }
  send_request(sockfd);
  loadImage("image.jpg");
}

void send_request(int sockfd){
  if(send(sockfd,location,strlen(location),0)<0) fprintf(stdout,"send location fail");
  usleep(1000);
  
  /*Receive size of file*/
  int size,n;
  if((n=recv(sockfd,recvBuff,sizeof(recvBuff),0))>0){
    recvBuff[n] = 0;
    fprintf(stdout,"Nhan dc size: %s\n",recvBuff);
    size = atoi(recvBuff);
    output_file = fopen("image.jpg","w");
    fclose(output_file);
    output_file = fopen("image.jpg","a");
  }
  
  /*Send ok ack*/
  char msg[30];
  memset(&msg,'\0', sizeof(msg));
  snprintf(msg,sizeof(msg),"%s","ok");
  if(send(sockfd,msg,strlen(msg),0)<0) fprintf(stdout,"send ack fail");
  
  int i=0;
  for (i;i<size;i++) {
    if ( (n = recv(sockfd, recvBuff, sizeof(recvBuff),0)) > 0) {
      fprintf(stdout,"Nhan goi: %d So n: %d\n",i,n);
      //fprintf(stdout,"%s",recvBuff);
      fwrite(recvBuff,1,n,output_file);
    }
  }

  fclose(output_file);
}

int main(int argc, char *argv[] ){
  gtk_init(&argc, &argv);
  
  gtkBuilder = gtk_builder_new();
  gtk_builder_add_from_file(gtkBuilder, "requestimage.glade", NULL);
  window = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "window"));
  
  /* Connect signals */
  gtk_builder_connect_signals( gtkBuilder, NULL );
  
  image = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"image"));
  text = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"curl"));
  
  //image = gtk_image_new_from_pixbuf(pixbuf);
  //gtk_image_set_from_file(image,"/home/hocphan/Pictures/panda2.png");
  loadImage("/home/hocphan/Pictures/panda2.png");
  
  g_object_unref(G_OBJECT(gtkBuilder));
  gtk_widget_show(window);
  gtk_main();

  return 0;
}