#include <stdio.h>
#include <stdlib.h>
#include "../util/util.h"
#include "wan.h"


int setFixWanIP(struct mosquitto *mosq, cJSON* data, char *topic)
{
    // complete your bussiness logic here. 
    return 0;
}

int module_init(){
    apte_handler_register("setFixWanIP",setFixWanIP);
}