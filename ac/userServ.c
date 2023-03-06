#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "public.h"


void testSql(){
    // SqlInsertStudent();
    // SqlQueryStudent();
}

void UserServMain(){

    SqlServInit();
    MqttSubInit();
    MqttPubInit();
    while (GetRunningState())
    {
        loginfo("userServMain\n");
        sleep(2);
        MqttPub("pub/test", "11111");
        // testSql();
    }

    SqlServExit();
    MqttPubExit();
    MqttSubExit();
    pthread_exit(0);
}