#include "lib.h"
#include <pthread.h>

#define NUM_THREADS 5

void* PrintHello(void *threadid)
{
   long tid;
   tid = (long)threadid;
   printf("Hello World! It's me, thread #%ld!\n", tid);
   pthread_exit(NULL);
}

void* does_not(void *a){
  int i = 0 ;
  for (i;i<8;i++) {
    //sleep(1);
    fprintf(stdout,"Does not! %i\n",i);
  }
  return NULL;
}

void* does_too(void *a){
  int i = 0 ;
  for (i;i<8;i++) {
    //sleep(1);
    fprintf(stdout,"Does too! %i\n",i);
  }
  return NULL;
}

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int beers = 0;
void* drink_lots(void *a){
  int i;
  //for (i=0;i<100000;i++) {
    long id = (long )a;
    for (;;) {
    pthread_mutex_lock(&lock);
    beers = beers + 1;
    fprintf(stdout,"ID: %ld___beers = %i\n",id,beers);
    usleep(100000);
    pthread_mutex_unlock(&lock);
  }  
  return NULL;
}

int main(int argc, char *argv){
  /*
   pthread_t threads[NUM_THREADS];
   int rc;
   long t;
   for(t=0; t<NUM_THREADS; t++){
      printf("In main: creating thread %ld\n", t);
      rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }

   /* Last thing that main() should do */
//   pthread_exit(NULL);

   /*
   pthread_t t0;
   pthread_t t1;
   if(pthread_create(&t0,NULL,does_not,NULL)==-1) error("thread t0 error");
   if(pthread_create(&t1,NULL,does_too,NULL)==-1) error("thread t1 error");
   void* result;
   if(pthread_join(t0,&result)==-1) error("t0 cant join");
   if(pthread_join(t1,&result)==-1) error("t1 cant join");
   exit(0);
   */
   
   pthread_t thread[20];
   int t;
   fprintf(stdout,"Beers: %i\n",beers);
   for (t=0;t<20;t++){
     if(pthread_create(&thread[t],NULL,drink_lots,(void *)t)<0) error("can not create thread");
   }
   void *result;
   for(t=0;t<20;t++){
     if(pthread_join(thread[t],&result)<0) error("can not join thread");
   }
   fprintf(stdout,"Beers left: %i\n",beers);
   return 0;
}