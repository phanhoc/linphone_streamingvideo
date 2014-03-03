#include "lib.h"

#define NUM_THREADS  3
#define TCOUNT 10
#define COUNT_LIMIT 12

int     count = 0;
int     thread_ids[3] = {0,1,2};
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

int out = 0;
char q[10];

void *inc_count(void *t) 
{
  int i;
  long my_id = (long)t;

  //for (i=0; i<TCOUNT; i++) {    
    //pthread_mutex_lock(&count_mutex);
    while(!out){
    
    
    fprintf(stdout,"Enter q to quit: ");
    fscanf(stdin,"%s",q);

    /* 
    Check the value of count and signal waiting thread when condition is
    reached.  Note that this occurs while mutex is locked. 
    */
    if (strstr(q,"q")) {
      out = 1;
      //pthread_cond_signal(&count_threshold_cv);
      fprintf(stdout,"Tin hieu out: %i\n", out);
    }
    else {
      fprintf(stdout,"Khong phai tin hieu out\n");
    }        
    }
    //pthread_mutex_unlock(&count_mutex);
    fprintf(stdout,"break thanh cong tin hieu\n");
  pthread_exit(NULL);
}

void *watch_count(void *t) 
{
  long my_id = (long)t;

  printf("Starting watch_count(): thread %ld\n", my_id);

  /*
  Lock mutex and wait for signal.  Note that the pthread_cond_wait 
  routine will automatically and atomically unlock mutex while it waits. 
  Also, note that if COUNT_LIMIT is reached before this routine is run by
  the waiting thread, the loop will be skipped to prevent pthread_cond_wait
  from never returning. 
  */
  
  while (!out) {    
    //pthread_cond_wait(&count_threshold_cv, &count_mutex);    
    pthread_mutex_lock(&count_mutex);
    count += 1;
    printf("watch_count(): thread %ld count now = %d.\n", my_id, count);    
    pthread_mutex_unlock(&count_mutex);
    usleep(250000);
    }      
    fprintf(stdout,"break thanh cong thread\n");
  pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
  int i, rc;
  long t1=1, t2=2, t3=3;
  pthread_t threads[3];
  pthread_attr_t attr;

  /* Initialize mutex and condition variable objects */
  pthread_mutex_init(&count_mutex, NULL);
  pthread_cond_init (&count_threshold_cv, NULL);

  /* For portability, explicitly create threads in a joinable state */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_create(&threads[0], &attr, inc_count, (void *)t1);
  pthread_create(&threads[1], &attr, watch_count, (void *)t2);
  pthread_create(&threads[2], &attr, watch_count, (void *)t3);

  /* Wait for all threads to complete */
  for (i=0; i<NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("Main(): Waited on %d  threads. Done.\n", NUM_THREADS);
  printf("Last count: %i\n",count);

  /* Clean up and exit */
  pthread_attr_destroy(&attr);
  pthread_mutex_destroy(&count_mutex);
  pthread_cond_destroy(&count_threshold_cv);
  pthread_exit(NULL);

}