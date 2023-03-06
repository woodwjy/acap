#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "public.h"


void testSql(){
    SqlInsertStudent();
    SqlQueryStudent();
}

void testPub(){
     MqttPub("pub/test", "11111");
}

void UserServMain(){

    SqlServInit();
    MqttPubInit();
    while (GetRunningState())
    {
        loginfo("userServMain\n");
        sleep(1);

        // testPub();
        // testSql();
    }

    SqlServExit();
    MqttPubExit();
    pthread_exit(0);
}