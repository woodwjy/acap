#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "public.h"

void ScriptManagerMain(){

    while (GetRunningState())
    {
        loginfo("ScriptManagerMain\n");
        sleep(2);
    }

    pthread_exit(0);
    
}