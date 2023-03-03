// 基于线程的并发编程,连接或分离线程
// Test_4_createThread
// 2019年10月27日14:45:11 尚未完全搞明白
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
 
#define NUM_Pthread 5
 
void *PrintHello(void * pthreadid)
{
    int tid = *((int *)pthreadid);
    printf("Sleeping in thread %d ,...exiting \n",tid);
    return 0;
}
 
int main(void)
{
    int i, ret;
    pthread_t Pthread[NUM_Pthread];
    pthread_attr_t attr; // 定义线程属性
    void * status;
    int index[NUM_Pthread];
 
    // 初始化并设置线程为可连接
    // pthread_attr_init(&attr);
    // pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
 
    for (i=0; i<NUM_Pthread; i++)
    {
        printf("main() : 创建线程 %d \n",i);
        index[i] = i;
        ret = pthread_create(&Pthread[i], NULL, PrintHello, (void *)&index[i]);
    }
 
    // 删除属性，并等待其他线程
    // pthread_attr_destroy(&attr);
    for (i=0; i<NUM_Pthread; i++)
    {
        ret = pthread_join(Pthread[i], status);
        if (0 != ret)
        {
            printf("Error: unable to join,%d\n",ret);
            exit(-1);
        }
        printf("main(): complete thread id : %d",i);
        printf(" exiting with status : %p\n",status);
    }
 
    printf("main() : program exiting.\n");
    pthread_exit(NULL);
 
    return 0;
}