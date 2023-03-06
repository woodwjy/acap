#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include "public.h"
#include "../util/util.h"

int PROGRAM_RUNNING_STATE = 1;

int GetRunningState(){
    return PROGRAM_RUNNING_STATE;
}


int sigHandler(int sig){
    switch (sig)
    {
        case SIGINT:
        case SIGQUIT:
        case SIGTERM:
            logwarn("Program Exit, sig=%d\n", sig);
            PROGRAM_RUNNING_STATE = 0;
            sleep(1);
            exit(0);
        default:
            break;
    }
}


#define THREAD_NUM 4
void pthreadInit(){
    int ret = 0;
    int index = 0;
    pthread_t tid[THREAD_NUM];
    void * status;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    ret = pthread_create(&tid[index++], NULL, &UserServMain, NULL);
    if ( ret != 0){
        logerr("pthread_create error: error_code = %d\n", ret);
    }


    ret = pthread_create(&tid[index++], NULL, &MqttSubMain, NULL);
    if ( ret != 0){
        logerr("pthread_create error: error_code = %d\n", ret);
    }

    ret = pthread_create(&tid[index++], NULL, &DiscServMain, NULL);
    if ( ret != 0){
        logerr("pthread_create error: error_code = %d\n", ret);
    }

    ret = pthread_create(&tid[index++], NULL, &ScriptManagerMain, NULL);
    if ( ret != 0){
        logerr("pthread_create error: error_code = %d\n", ret);
    }

    ret = pthread_create(&tid[index++], NULL, &ApManagerMain, NULL);
    if ( ret != 0){
        logerr("pthread_create error: error_code = %d\n", ret);
    }


    pthread_attr_destroy(&attr);
    for (int i=0; i<THREAD_NUM; i++){
        if (0 != pthread_join(tid[i], status) ){
            logerr("Error: unable to join\n");
            exit(-1);
        }
    }

}

void main(){

    
    signal(SIGINT , sigHandler);
    signal(SIGQUIT , sigHandler);
    signal(SIGTERM ,  sigHandler);

    loginfo("%s\n", "hello!");
    funcUtil();

    pthreadInit();

}