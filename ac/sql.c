#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include "public.h"
#include "../ext/sqlite3.h"
#include "public.h"
#include "sql.h"

#define SQLITE_DB_NAME "acap.db"
sqlite3 *db;

int callback(void * para,int n_column,char** data_column,char** name_column){
    int i; 

    for (i=0; i<n_column; i++) 
    { 
        printf("%-8s", name_column[i]); 
    } 
        printf("\n"); 

    for (i=0; i<n_column; i++) 
    { 
        printf("%-8s", data_column[i]); 
    } 
        printf("\n"); 
    
    return 0;
}

void SqlSrvInit(){
    int ret;
    ret = sqlite3_open(SQLITE_DB_NAME, &db);
    if (ret == SQLITE_OK){
        loginfo("open acap.db success!\n");
    } else {
        logerr("error:%s, %d\n", sqlite3_errmsg(db), ret);
        return NULL;
    }


    ret = sqlite3_exec(db, CREATE_PERSON_TABLE, 0, 0, 0);
    if (ret != SQLITE_OK){
        logerr("SQL Error :%s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    return NULL;

}

void SqlInsertStudent(){

    char sql[1024] = {0}; 

    loginfo("xxx\n");
    loginfo("%s\n",FORMAT_INSERT_STUDENT);
    sprintf(sql, FORMAT_INSERT_STUDENT, "xiaoming", 17);

    loginfo("%s\n",sql);

    int ret = sqlite3_exec(db, sql, 0, 0, 0);
    if (ret != SQLITE_OK){
        logerr("SQL Error :%s\n", sqlite3_errmsg(db));
        return NULL;
    }
}

void SqlQueryStudent(){
    char * sql = "SELECT * FROM test_table;";
    int ret = sqlite3_exec(db, sql, callback, 0, 0);
    if (ret != SQLITE_OK) {
        logerr("SQL error: %s\n", sqlite3_errmsg(db));
    }
}

void SqlSrvExit(){
    if(db != NULL){
        loginfo("sqlite3_close\n");
        sqlite3_close(db);
    }
}