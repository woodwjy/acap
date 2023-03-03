#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "public.h"

void DiscServMain(){

    while (GetRunningState())
    {
        loginfo("DiscServMain\n");
        sleep(2);
    }

    pthread_exit(0);
    
}