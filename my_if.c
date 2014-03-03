#define _GNU_SOURCE     /* To get defns of NI_MAXSERV and NI_MAXHOST */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_link.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>
#include "lib.h"

int main(int argc, char *argv[]) {
  struct ifaddrs *ifaddr, *ifa;
  int family, n;
  char host[NI_MAXHOST];
  char net[NI_MAXHOST];
  char broad[NI_MAXHOST];
  
  int sockfd = 0;
  char msg[MAXMSG];
  size_t size;
  int broadcast = 1;
  struct sockaddr_in serv_addr;
  
  if (getifaddrs(&ifaddr) == -1) {
    perror("getifaddrs");
    exit(EXIT_FAILURE);
  }
  
  /* Walk through linked list, maintaining head pointer so we
   *      can free list later */
  
  for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
    if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET || strstr(ifa->ifa_name,"lo") )
      continue;
    
    fprintf(stdout,"Name of interface: %s\n",ifa->ifa_name);
    //fprintf(stdout,"Flags: %i\n",ifa->ifa_flags);
    //ifa->ifa_flags = ifa->ifa_flags & IFF_BROADCAST;
    if(getnameinfo(ifa->ifa_addr,
      //(family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
		   sizeof(struct sockaddr_in),
       host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST)<0) {
	 perror("get host");
	 exit(EXIT_FAILURE);
       }
       fprintf(stdout,"IP: %s\n",host);
       
       if(getnameinfo(ifa->ifa_netmask,
	 //(family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
		      sizeof(struct sockaddr_in),
	  net, NI_MAXHOST, NULL, 0, NI_NUMERICHOST)<0) {
	    perror("get ip");
	    exit(EXIT_FAILURE);
	  }
	  fprintf(stdout,"Netmask: %s\n",net);
	  
	  if (getnameinfo(ifa->ifa_ifu.ifu_broadaddr,
	    (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
			  broad, NI_MAXHOST, NULL, 0, NI_NUMERICHOST)<0) {
	    perror("get broadcast");
	  exit(EXIT_FAILURE);
			  }
			  fprintf(stdout,"Broadcast: %s\n",broad);
			  
			  // khoi tao socket cho client
			  if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
			    error("Khong tao dc socket");
			  }
			  
			  if(setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof(broadcast))<0) {
			    error("Khong broadcast dc");
			  }
			  
			  memset(&serv_addr,'0',sizeof(serv_addr));
			  serv_addr.sin_family = AF_INET;
			  serv_addr.sin_port = htons(PORT);
			  
			  if(inet_pton(AF_INET,broad,&serv_addr.sin_addr)<=0) {
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
			  
			  
#ifdef PROG
			  family = ifa->ifa_addr->sa_family;
			  
			  /* Display interface name and family (including symbolic
			   form of* the latter for the common families) */
			  
			  printf("%-8s %s (%d)\n",
			  ifa->ifa_name,
			  (family == AF_PACKET) ? "AF_PACKET" :
			  (family == AF_INET) ? "AF_INET" :
			  (family == AF_INET6) ? "AF_INET6" : "???",
			   family);
			  
			  /* For an AF_INET* interface address, display the address */
			  
			  if (family == AF_INET || family == AF_INET6) {
			    s = getnameinfo(ifa->ifa_addr,
					    (family == AF_INET) ? sizeof(struct sockaddr_in) :
					    sizeof(struct sockaddr_in6),
					    host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			    if (s != 0) {
			      printf("getnameinfo() failed: %s\n", gai_strerror(s));
			  exit(EXIT_FAILURE);
			    }
			    
			    printf("\t\taddress: <%s>\n", host);
			  
			  } else if (family == AF_PACKET && ifa->ifa_data != NULL) {
			    struct rtnl_link_stats *stats = ifa->ifa_data;
			    
			    printf("\t\ttx_packets = %10u; rx_packets = %10u\n"
			    "\t\ttx_bytes   = %10u; rx_bytes   = %10u\n",
			    stats->tx_packets, stats->rx_packets,
			    stats->tx_bytes, stats->rx_bytes);
			  }
#endif	   
  }
  
  
  freeifaddrs(ifaddr);
  exit(EXIT_SUCCESS);
}

