#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

#define PORT 8000
#define MAXMSG 1024

char recvBuff[MAXMSG];
char packet[MAXMSG];
char *keysearch;

void recv_file(int connfd){
  int n;
  FILE *fd;

  /*Nhan duoc location tim trong co so du lieu*/
  if((n=recv(connfd,recvBuff,sizeof(recvBuff),0))>0){
    recvBuff[n] = 0;
    fprintf(stdout,"Nhan dc location: %s\n",recvBuff);
    keysearch = strdup(recvBuff);
    /*Tim kiem trong co so du lieu--->K55 lam
     * Hien tai K54 doc file anh ra va gui tra lai
     */
    fd = fopen("/home/hocphan/Pictures/giaothong2.jpg","r");
    fseek(fd,0,SEEK_END);
    int size = ftell(fd);
    fseek(fd,0,SEEK_SET);
    if (size%MAXMSG) size = size/MAXMSG + 1;
    else size = size/MAXMSG;
    fprintf(stdout,"Packets : %i\n",size);
    
    /*Send size for client*/
    char msg[30];
    memset(&msg,'\0', sizeof(msg));
    snprintf(msg,sizeof(msg),"%i",size);
    if (send(connfd,msg,strlen(msg),0)<0) fprintf(stdout,"send size loi");
    usleep(1000);
  
  }
  
  /*Receive ok and send file to client*/
  if((n=recv(connfd,recvBuff,sizeof(recvBuff),0))>0){
    recvBuff[n] = 0;
    fprintf(stdout,"Nhan dc ack: %s\n",recvBuff);
    int count = 0;
    while(!feof(fd)){
      int wasRead = fread(packet,sizeof(char),MAXMSG,fd);
      send(connfd,packet,(sizeof(char)*wasRead),0);
      fprintf(stdout,"Count: %i\n",count);
      count++;    
      usleep(1000);
    }
  }
}

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[MAXMSG];
    char okBuff[MAXMSG] = "ok";
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);	// tao un-named socket, tra ve int socket descriptor 
							// AF_INET : domain IPv4
							// SOCK_STREAM,0 :TCP
    memset(&serv_addr, '0', sizeof(serv_addr));	
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;			// AF_INET : domain IPv4
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// cho phep cac IP den
    serv_addr.sin_port = htons(PORT); 			// setup port
    
    int reuse = 1;
    if (setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(char *)&reuse,sizeof(int)) == -1) fprintf(stdout,"Cant set reuse");

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 	// chi tiet ve socket <--> serv_addr

    listen(listenfd, 10); 	// max client:10
    
    memset(recvBuff, '\0', sizeof(recvBuff)); 
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 	// wait client
									// return socket descriptor representing the client socket
/*
        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff)); 	// send chuoi >> connfd
*/	

	recv_file(connfd);

/*
	int n;
	FILE *output_file;	
	char *name;
	
	if((n=recv(connfd,recvBuff,sizeof(recvBuff),0))>0){
	  recvBuff[n] = 0;
	  fprintf(stdout,"Nhan dc name: %s\n",recvBuff);
	  name = strdup(recvBuff);
	  output_file = fopen(recvBuff,"w");
	  fclose(output_file);
	  output_file = fopen(recvBuff,"a");
	}
	
	int size;
	if((n=recv(connfd,recvBuff,sizeof(recvBuff),0))>0){
	  recvBuff[n] = 0;
	  fprintf(stdout,"Nhan dc size: %s\n",recvBuff);
	  size = atoi(recvBuff);
	}
	
	int i=0;
	for (i;i<size;i++) {
	  if ( (n = recv(connfd, recvBuff, sizeof(recvBuff),0)) > 0) {
	    fprintf(stdout,"Nhan goi: %d So n: %d\n",i,n);
	    //fprintf(stdout,"%s",recvBuff);
	    fwrite(recvBuff,1,n,output_file);
	  }
	}
	
	fclose(output_file);
	fprintf(stdout,"Success receive file: %s\n",name);
*/
    }
	/*
	char file_name[1024];
	if ( (n = read(connfd, recvBuff, sizeof(recvBuff))) > 0)	// nhan ten file
	{
	  recvBuff[n] = 0;
	  fprintf(stdout,"nhan dc ten: %s",recvBuff);
	  fflush(stdout);
	  //file_name = recvBuff;
	  //if(fputs(recvBuff, stdout) == EOF)
	  //{
          //  printf("\n Error : Fputs error\n");
	  //}
	}
	else {
	  fprintf(stdout,"loi");
	}
	memset(recvBuff, '0', sizeof(recvBuff)); 
	int packet = 0;
	if ( (n = read(connfd, recvBuff, sizeof(recvBuff))) > 0)	// nhan so packet
	{	  
	  fprintf(stdout,"doc dc: %i\n",n);
	  fflush(stdout);
	  recvBuff[n] = 0;
	  //sprintf(recvBuff,"%s",recvBuff);
	  packet = atoi(recvBuff);
	  fprintf(stdout,"nhan dc goi: %s",recvBuff);
	  fflush(stdout);
	  fprintf(stdout,"nhan dc goi int: %i\n\n\n",packet);	  
	  fflush(stdout);
	}
	else {
	  fprintf(stdout,"loi");
	}
	*/
	
	
	/*
	
	FILE *output_file = fopen("Giao_trinh_C++.pdf","w");
	fclose(output_file);
		
	output_file = fopen("Giao_trinh_C++.pdf","a");
	int i = 0;
	
	fflush(stdout);
	//for (i;i<packet;i++) {
	for (i;i<1;i++) {
	  if ( (n = read(connfd, recvBuff, sizeof(recvBuff))) > 0) {
	    fprintf(stdout,"%s",recvBuff);
	    fflush(stdout);
	    fwrite(recvBuff,1,n,output_file);
	    //memset(recvBuff, 0 , sizeof(recvBuff));
	  }
	  //usleep(10000);
	}
	
	fclose(output_file);
	
	*/
	
	//if ( (n = recv(connfd, recvBuff, sizeof(recvBuff),0)) > 0) fprintf(stdout,"%s",recvBuff);
	
	/*
	for (i;i<packet;i++) {
	  if ( (n = recv(connfd, recvBuff, sizeof(recvBuff),0)) > 0)	// nhan so packet
	  {
	    fprintf(stdout,"%i ",n);
	    fwrite(recvBuff,1,n,output_file);
	    //rewind(output_file);
	    write(connfd, okBuff, strlen(okBuff));
	  }
	  else {
	    fprintf(stdout,"%i\n",n);
	    fprintf(stdout,"read loi\n");
	  }
	}
	
	
	fclose(output_file);
	*/
	
        //close(connfd);	// close >> connfd
        //sleep(1);
}
