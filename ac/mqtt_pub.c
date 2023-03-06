#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "public.h"
#include "mosquitto.h"
#include "sql.h"


#define HOST "localhost"
#define PORT 1883
#define KEEP_ALIVE 60
#define MSG_MAX_SIZE 512
#define MQTT_ID "ac-pub"


static int running = 1;


void my_pub_connect_callback(struct mosquitto *mosq,void *obj,int rc)
{
    printf("Call the function: my_pub_connect_callback.\n");
}

void my_pub_disconnect_callback(struct mosquitto *mosq,void *obj,int rc)
{
    printf("Call the function: my_pub_disconnect_callback.\n");
    running = 0;

}


void my_publish_callback(struct mosquitto *mosq,void *obj,int mid)
{
    printf("Call the function: my_publish_callback.\n");

}


struct mosquitto *getMosqInstance ()
{

    static struct mosquitto *mosq = NULL;

    if (NULL != mosq){
        return mosq;
    } else {
        int rv;

        mosquitto_lib_init();

        mosq = mosquitto_new(MQTT_ID,true,NULL);
        if(mosq == NULL)
        {
            printf("New pub_test error: %s\n",strerror(errno));
            mosquitto_lib_cleanup();
            return NULL;
        }

        mosquitto_connect_callback_set(mosq,my_pub_connect_callback);
        mosquitto_disconnect_callback_set(mosq,my_pub_disconnect_callback);
        mosquitto_publish_callback_set(mosq,my_publish_callback);

        rv = mosquitto_connect(mosq,HOST,PORT,KEEP_ALIVE);

        if(rv)
        {
            printf("Connect server error: %s\n",strerror(errno));
            mosquitto_destroy(mosq);
            mosquitto_lib_cleanup();
            return NULL;
        }

        printf("Start!\n");

        int loop = mosquitto_loop_start(mosq);
        if(loop)
        {
            printf("mosquitto loop error: %s\n",strerror(errno));
            return NULL;
        }

        return mosq;
    }
} 

void MqttPubInit(){
    printf("MqttPubInit");
    struct mosquitto *mosq = getMosqInstance();
    printf("MqttPubInit");
}

void MqttPubExit(){
    struct mosquitto *mosq = getMosqInstance();
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

void MqttPub(char *topic, char *payload){
    struct mosquitto *mosq = getMosqInstance();
    printf("MqttPub");
    mosquitto_publish(mosq,NULL,topic,strlen(payload)+1, payload,0,0);
}