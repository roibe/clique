
#include "cliques.h"

void *threadFunc(void *arg){get_cliques(atoi((char*)arg));return NULL;}

int main(int argc, char** argv)
{
  pthread_t thread1, thread2;
  clock_t t1;
  construct(argc, argv);
  t1 = clock();
  pthread_create(&thread1, NULL, threadFunc, "1");
  pthread_create(&thread2, NULL, threadFunc, "2");
  
  pthread_join(thread1, NULL /* void ** return value could go here */);
  pthread_join(thread2, NULL /* void ** return value could go here */);
printf("Alg: %lf  ms\n", (double) (clock()-t1)/1000);
  free_all();
  
  return 0;
}
