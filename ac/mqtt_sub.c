#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "public.h"
#include "mosquitto.h"
#include "sql.h"

#define HOST "localhost"
#define PORT 1883
#define KEEP_ALIVE 60
#define MQTT_ID "ac-sub"

static int running = 1;

typedef struct
{
    char *topic;
    void (*handler)();
} topicHandler;

void usertopic1handler(){
    loginfo("xx\n");
}

void usertopic2handler(){
    loginfo("yy\n");
}

void usertopic3handler(){
    loginfo("zz\n");
}

topicHandler thTable[] = {
    {"user/topic/1", usertopic1handler},
    {"user/topic/2", usertopic2handler},
    {"user/topic/3", usertopic3handler},
};

int thTableLength = sizeof(thTable)/sizeof(thTable[0]);

void my_connect_callback(struct mosquitto *mosq,void *obj,int rc)
{
    loginfo("Call the function:on_connect\n");

    if(rc)
    {
        logerr("on_connect error!\n");
        exit(1);
    }
    else
    {
        for (int i = 0 ; i < thTableLength ; i ++ ) {
            if(mosquitto_subscribe(mosq,NULL,thTable[i].topic,2))
            {
                logerr("Set the topic error!\n");
                exit(1);
            }
        }

    }
}


void my_disconnect_callback(struct mosquitto *mosq,void *obj,int rc)
{
    loginfo("Call the function: my_disconnect_callback\n");
    running = 0;
}

void my_subscribe_callback(struct mosquitto *mosq,void *obj,int mid,int qos_count,const int *granted_qos)
{
    loginfo("Call the function: on_subscribe\n");
}

void my_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    loginfo("Call the function: on_message\n");
    loginfo("Recieve a message of %s: %s\n.",(char *)msg->topic,(char *)msg->payload);
    if(0 == strcmp(msg->payload,"quit")){
    
        mosquitto_disconnect(mosq);
    }

    for (int i = 0 ; i < thTableLength ; i ++ ) {
        if (0 == strcmp((char *)msg->topic, thTable[i].topic)){
            thTable[i].handler();
        }
    }

}

int MqttSubMain()
{
    int ret;
    struct mosquitto *mosq;

    ret = mosquitto_lib_init();

    mosq = mosquitto_new(MQTT_ID,true,NULL);
    if(mosq == NULL)
    {
        logerr("New sub_test error!\n");
        mosquitto_lib_cleanup();
        return -1;
    }

    
    mosquitto_connect_callback_set(mosq,my_connect_callback);
    mosquitto_disconnect_callback_set(mosq,my_disconnect_callback);
    mosquitto_subscribe_callback_set(mosq,my_subscribe_callback);
    mosquitto_message_callback_set(mosq,my_message_callback);

    ret = mosquitto_connect(mosq,HOST,PORT,KEEP_ALIVE);

    if(ret)
    {
        logerr("Connect server error!\n");
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return -1;
    }

    loginfo("Start!\n");

    while(GetRunningState() && running)
    {
        mosquitto_loop(mosq,-1,1);
        //mosquitto_loop_start(mosq);
    }

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
} 
