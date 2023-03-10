#include <stdio.h>
#include <stdlib.h>
#include "../util.h"
#include "apte_list.h"
#include "system.h"



int templateSet(struct mosquitto *mosq, cJSON* data, char *topic)
{
    return 0;
}

int module_init(){
	apte_handler_register("templateSet",templateSet);
}