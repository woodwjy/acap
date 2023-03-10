#include <stdio.h>
#include <stdlib.h>
#include "../util.h"
#include "../apte_list.h"
#include "wifi.h"


int getWiFiApInfo(struct mosquitto *mosq, cJSON* data, char *topic)
{
    loginfo("111\n");
    return 0;
}

int getWiFiStaInfo(struct mosquitto *mosq, cJSON* data, char *topic)
{
    loginfo("222\n");
    return 0;
}

int module_init(){
    loginfo("xxxr\n");
	apte_handler_register("getWiFiApInfo",getWiFiApInfo);
    apte_handler_register("getWiFiStaInfo",getWiFiStaInfo);
}