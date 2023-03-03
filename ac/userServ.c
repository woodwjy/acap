#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "public.h"
#include "sql.h"


void testSql(){
    SqlInsertStudent();
    SqlQueryStudent();
}

void UserServMain(){

    SqlSrvInit();
    while (GetRunningState())
    {
        loginfo("userServMain\n");
        sleep(2);
        // testSql();
    }

    SqlSrvExit();
    pthread_exit(0);
    
}