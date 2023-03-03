#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "public.h"

void UserServMain(){

    while (GetRunningState())
    {
        loginfo("userServMain\n");
        sleep(2);
    }

    pthread_exit(0);
    
}