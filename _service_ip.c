#include "lib.h"

int out = 0;
char q[10];

pthread_t waiting_thread;
pthread_t killing_thread;

FILE *fd;

/*Ham phuc vu thread waiting*/
void *waiting_handle(void *t){
  int sockfd = 0;
  char msg[MAXMSG];
  size_t size;
  struct sockaddr_in serv_addr;
  struct sockaddr_in clie_addr;
  
  if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0) {
    error("Khong tao dc socket");
  }
  
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(&clie_addr, '0', sizeof(clie_addr));
  memset(msg,'0',sizeof(msg));
  
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(PORT);
  
  if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0) {
    error("Khong bind dc socket");
  }
  
  fprintf(stdout,"Waiting for client...\n");
  
  size = sizeof(clie_addr);
  while(!out) {     
    if(recvfrom(sockfd,msg,MAXMSG,0,(struct sockaddr*)&clie_addr,&size)<0) {
      error("Khong nhan dc ban tin");
    }
    
    //fprintf(stdout,"Got msg: %s\n",msg);
    
    if (strstr(msg,"REQUEST")) {
      fprintf(stdout,"%s",now());
      fprintf(stdout,"%s From: \nIP: %s\n\n",msg,inet_ntoa(clie_addr.sin_addr));
      clie_addr.sin_port = htons(PORT);
      /*
       * Luu lai IP o day
       */
      write_file("ip_table.txt",inet_ntoa(clie_addr.sin_addr));
      if(sendto(sockfd,MSG2,sizeof(MSG2),0,(struct sockaddr*)&clie_addr,sizeof(clie_addr)) < 0) {
	error("Khong gui dc");
      }
    }
    else if (strstr(msg,"RESPONSE")) {
    //fprintf(stdout,"\nRESPONSE\nFrom \nIP: %s\nPort: %i\n",inet_ntoa(clie_addr.sin_addr),clie_addr.sin_port);  
      fprintf(stdout,"%s",now());
      fprintf(stdout,"%s From: \nIP: %s\n\n",msg,inet_ntoa(clie_addr.sin_addr));
      write_file("ip_table.txt",inet_ntoa(clie_addr.sin_addr));
    }
    else {
      fprintf(stdout,"Khong ho tro ban tin! \n");
    }              
  }
  fprintf(stdout,"Waiting done... Byebye !\n");
  pthread_exit(NULL);
}

/*Ham phuc vu thread killing*/
void *killing_handle(void *t){
  char q[5];
  while(!out){
    fprintf(stdout,"Enter q to quit:\n");
    fscanf(stdin,"%s",q);
    if(strstr(q,"q")) {
      out = 1;
      fprintf(stdout,"Tin hieu kill\n");
    }
    else fprintf(stdout,"Khong phai tin hieu kill\n");
  }
  fprintf(stdout,"Killing done... Byebye !\n");
  pthread_cancel(waiting_thread);
  pthread_exit(NULL);
}

/*Ham broadcast all interface*/
void broadcast_request_all_interface() {
  /*
   * Set up ban tin request broadcast >> all interface
   */
  struct ifaddrs *ifaddr, *ifa;
  int n;
  char host[NI_MAXHOST];
  //char net[NI_MAXHOST];
  char broad[NI_MAXHOST];
  
  int sockfd = 0;
  int broadcast = 1;
  char msg[MAXMSG];
  struct sockaddr_in serv_addr;
  
  if (getifaddrs(&ifaddr) == -1) {
    error("Cant get interface");
  }
  
  for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
    if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET || strstr(ifa->ifa_name,"lo") )
      continue;
    if(getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host,NI_MAXHOST,NULL,0,NI_NUMERICHOST)<0) {
      error("cant get host");
    }
    if(getnameinfo(ifa->ifa_ifu.ifu_broadaddr,sizeof(struct sockaddr_in),broad,NI_MAXHOST,NULL,0,NI_NUMERICHOST)<0) {
      error("cant get broadcast");
    }
    fprintf(stdout,"\nBroadcast interface: %s\n",ifa->ifa_name);
    fprintf(stdout,"IP: %s\n",host);
    fprintf(stdout,"Broadcast: %s\n\n",broad);
    
    if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0){
      error("Khong tao dc socket");
    }
    
    if (setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof(broadcast))<0){
      error("Khong broadcast dc");
    }
    
    memset(&serv_addr,'0',sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if(inet_pton(AF_INET,broad,&serv_addr.sin_addr)<=0) {
      error("Khong convert dc IP broadcast");
    }
    
    if(sendto(sockfd,MSG1,sizeof(MSG1),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0) {
      error("Khong sendto dc");
    }    
    close(sockfd);      
  }  
}

int main(int argc,char *argv){
  
  /*
   * Clear file
   */
  fd = fopen("ip_table.txt","w");
  fclose(fd);
  /*
   * Set up thread
   */
  if(pthread_create(&waiting_thread,NULL,waiting_handle,NULL)<0){
    error("Cant create thread waiting");
  }
  if(pthread_create(&killing_thread,NULL,killing_handle,NULL)<0){
    error("Cant create thread kill");
  }
  
  broadcast_request_all_interface();
    
  /*
   * Join thread
   */
  void *result;
  if(pthread_join(waiting_thread,&result)<0) error("cant join thread waiting");
  if(pthread_join(killing_thread,&result)<0) error("cant join thread killing");    
  
  fprintf(stdout,"Out service\n");
  pthread_exit(NULL);
}