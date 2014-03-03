#include "lib.h" 

int main(int argc, char *argv[]) {
  int sockfd = 0;
  char msg[MAXMSG];
  size_t size;
  int broadcast = 1;
  struct sockaddr_in serv_addr;
  
  if(argc !=2) {
    error("Chua nhap host");
  }
  
  // khoi tao socket cho client
  if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
    error("Khong tao dc socket");
  }
  
  if(setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof(broadcast))<0) {
    error("Khong broadcast dc");
  }
  
  // thiet lap socket server
  memset(&serv_addr,'0',sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  
  if(inet_pton(AF_INET,argv[1],&serv_addr.sin_addr)<=0) {
    error("Khong conver dc IP");
  }
  
  if(sendto(sockfd,MSG1,sizeof(MSG1),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0) {
    error("Khong gui dc");
  }
  
  if(recvfrom(sockfd,msg,MAXMSG,0,NULL,0)<0) {
    error("Khong nhan dc ban tin");
  }
  
  fprintf(stdout,"Got msg: %s\n",msg);
  
  fprintf(stdout,"Out client\n");
  
  close(sockfd);
  exit(0);
}