#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "mosquitto.h"
#include "util.h"
#include "apte_list.h"

#define HOST "localhost"
#define PORT 1883
#define KEEP_ALIVE 60
#define MQTT_ID "apte"

static int running = 1;

typedef struct
{
    char *topic;
    int (*handler)(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message);
} topicHandler;

int topic_quit_handler(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message){
    loginfo("mqtt quit...\n");
    mosquitto_disconnect(mosq);
    return 0;
}

topicHandler thTable[] = {
    {"topic/quit", topic_quit_handler},
    {"topic/apte/#", NULL},
};

int thTableLength = sizeof(thTable)/sizeof(thTable[0]);

int get_sub_topic(char* topic, char* action)
{
    int i;
    for(i=strlen(topic);i>1;i--){
        if(topic[i]=='/'){
            strcpy(action,&topic[i+1]);
            break;
        }
    }
    return strlen(action);
}

int apte_process_msg(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
    char subtp[64]={0};
    int i=0, found=0, ret=0;

    cJSON *root = cJSON_Parse(message->payload);
    if(!root){
        logerr(" cJSON_Parse error!\n");
        return -1;
    }
    
    i = strlen(message->topic);
    if(!('/' == message->topic[i-2] && 'R' == message->topic[i-1]))
    {
        get_sub_topic(message->topic, subtp);           

        apte_handler  *curhook;
        curhook = apte_handler_list;
        while (NULL != curhook) {
            loginfo("[test]%s\n", curhook->name);
            if(0==strcmp(subtp, curhook->name)){
                ret =(*curhook->handler)(mosq,root,message->topic);
                found = 1;
                break;  
            }
            
            ret = -1;
            curhook = curhook->next;
        }

        if(1 != found){
            logwarn("Handler not found, unsurported topic: %s\n module!*", message->topic);
        }
    }

    cJSON_Delete(root);

    return 0;
}


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
        apte_handler  *curhook;
        curhook = apte_handler_list;

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
    
    int found = 0;

    if(0 == strcmp(msg->payload,"quit")){
    
        mosquitto_disconnect(mosq);
    }


    int (*pfunc)() = NULL;

    for (int i = 0 ; i < thTableLength ; i ++ ) {
        if (0 == strcmp((char *)msg->topic, thTable[i].topic)){
            pfunc = thTable[i].handler(mosq, obj, msg);
            break;
        }
    }

    if(!pfunc){
        apte_process_msg(mosq, obj, msg);
    }

}

int mqtt_init()
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

    while(running)
    {
        mosquitto_loop(mosq,-1,1);
        // mosquitto_loop_forever(mosq, -1, 1);
    }

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
} 
