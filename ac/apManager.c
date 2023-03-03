#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "public.h"

void ApManagerMain(){

    while (GetRunningState())
    {
        loginfo("ApManagerMain\n");
        sleep(2);
    }

    pthread_exit(0);
    
}