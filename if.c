/*
* GNU/Linux C Program to return a list of devices and their IP Addresses
* Mark Loiseau - 2012
* http://blog.markloiseau.com/2012/02/get-network-interfaces-in-c/
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/netdevice.h>
#include <arpa/inet.h>
 
#define MAXINTERFACES 20
 
// Print errors and exit
void diep(char *s)
{
    perror(s);
    exit(1);
}
 
int main(int argc, char *argv[])
{
    int sock;
    struct ifconf ifconf;
    struct ifreq ifreq[MAXINTERFACES];
    int interfaces;
    int i;
 
    // Create a socket or return an error.
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        diep("socket");
 
    // Point ifconf's ifc_buf to our array of interface ifreqs.
    ifconf.ifc_buf = (char *) ifreq;
    
    // Set ifconf's ifc_len to the length of our array of interface ifreqs.
    ifconf.ifc_len = sizeof ifreq;
 
    //  Populate ifconf.ifc_buf (ifreq) with a list of interface names and addresses.
    if (ioctl(sock, SIOCGIFCONF, &ifconf) == -1)
        diep("ioctl");
 
    // Divide the length of the interface list by the size of each entry.
    // This gives us the number of interfaces on the system.
    interfaces = ifconf.ifc_len / sizeof(ifreq[0]);
 
    // Print a heading that includes the total # of interfaces.
    printf("IF(%d)tIPn\n", interfaces);
    
    // Loop through the array of interfaces, printing each one's name and IP.
    for (i = 0; i < interfaces; i++) {
        char ip[INET_ADDRSTRLEN];
        struct sockaddr_in *address = (struct sockaddr_in *) &ifreq[i].ifr_addr;
 
        // Convert the binary IP address into a readable string.
        if (!inet_ntop(AF_INET, &address->sin_addr, ip, sizeof(ip)))
            diep("inet_ntop");
 
        printf("Name: %s\nIP: %s\n", ifreq[i].ifr_name, ip);
    }
 
    close(sock);
 
    exit(0);
}