

#include <stdio.h>
#include <pthread.h>


void * THreadFun(void *arg)
{
    char* str = (char *) arg;

    printf("thread arg %s \n",str);
}

int main()
{
    pthread_t th_id;

    char* name = "mythread";

  int ret;
  ret =  pthread_create(&th_id,NULL,THreadFun,name);

  if(ret)
  {
    perror("pthred create error");
  }


  pthread_join(th_id,NULL);

  return 0;




}