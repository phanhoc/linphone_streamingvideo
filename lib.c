#include "lib.h"

void error(char *msg) {
  fprintf(stderr,"%s: %s\n",msg,strerror(errno));
  exit(1);
}

char* now() {
  time_t t;
  time (&t);
  return asctime(localtime (&t));
}

void write_file(char *file_name,char *msg){
  FILE *fd = fopen(file_name,"a");
  if (fd == NULL) {
    error("cant open file");    
  }
  fprintf(fd,"%s\n",msg);
  fclose(fd);
}

int check_ip(char *file_name,char *ip){
  FILE *fd = fopen(file_name,"r");
  char buf[100];
  if (fd == NULL) {
    error("cant open file");    
  }
  while(fgets(buf,100,fd)!=NULL) {
    if (strstr(buf,ip)) {
      fclose(fd);
      return 1;
    }
  }
  fclose(fd);
  return 0;
}