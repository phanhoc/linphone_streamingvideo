#include "lib.h"

int main(int argc, char *argv[]) {
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
  while(1) {     
    if(recvfrom(sockfd,msg,MAXMSG,0,(struct sockaddr*)&clie_addr,&size)<0) {
      error("Khong nhan dc ban tin");
    }
    
    fprintf(stdout,"Got msg: %s\n",msg);
    fprintf(stdout,"Thong so client\nSize: %i\nIP: %s\nPort: %i\n",size,inet_ntoa(clie_addr.sin_addr),clie_addr.sin_port);
    
    if(sendto(sockfd,MSG2,sizeof(MSG2),0,(struct sockaddr*)&clie_addr,sizeof(clie_addr)) < 0) {
      error("Khong gui dc");       
    }          
  }
}