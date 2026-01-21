




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

    pthread_attr_t attr;

    size_t stack_size;


    pthread_attr_init(&attr);

    char* name = "mythread";

    pthread_attr_getstacksize(&attr,&stack_size);

    printf("default stack %d \n",stack_size);



  int ret;
  ret =  pthread_create(&th_id,&attr,THreadFun,(void*)name);

  if(ret)
  {
    perror("pthred create error");
  }


  pthread_join(th_id,NULL);

  return 0;




}