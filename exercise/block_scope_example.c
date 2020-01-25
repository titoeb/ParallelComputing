        #include <pthread.h>
#include <stdio.h>    // For printf()
#include <stdlib.h>   // For exit()
#include <string.h>   // For strlen()
#include <unistd.h>   // For sleep() if needed


int *ptr;


static void
useless_count(void)
{
  int count = 0;
  ptr = &count;

  count++;
}


static void
*code_for_thread1(void *arg)
{
  printf("count in thread 1 is initially %d\n", *ptr);
  useless_count();
  printf("count in thread 1 is finally   %d\n", *ptr);
  return NULL;
}


int
main(void)
{
  pthread_t thread1;
  void      *thread1_result;

  // What happens if you remove the following statement?
  useless_count();
  printf("count in main thread is initially %d\n", *ptr);

  pthread_create(&thread1, NULL, code_for_thread1, NULL);
  pthread_join(thread1, &thread1_result);
  
  printf("count in main thread is finally   %d\n", *ptr);
  exit(0);
}
    
