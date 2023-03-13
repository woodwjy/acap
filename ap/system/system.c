#include <stdio.h>
#include <stdlib.h>
#include "../util/util.h"
#include "system.h"


int setPassword(struct mosquitto *mosq, cJSON* data, char *topic)
{
    char_t *pass = getJsonVar(data, "passwd", DEFAULT_PASSWD);
    // complete your bussiness logic here. 

    loginfo("111\n");
    return 0;
}

int getPassword(struct mosquitto *mosq, cJSON* data, char *topic)
{
    loginfo("111\n");
    return 0;
}

int rebootSystem(struct mosquitto *mosq, cJSON* data, char *topic)
{

    loginfo("111\n");
    return 0;
}

int module_init(){
    loginfo("system module init.\n");
	apte_handler_register("setPassword",setPassword);
	apte_handler_register("getPassword",getPassword);
    apte_handler_register("rebootSystem", rebootSystem);
}