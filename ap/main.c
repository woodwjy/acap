#include <stdio.h>
#include <stdlib.h>
#include "mqtt.h"
#include "module.h"
#include "apte_list.h"

#include "util.h"




int func1(struct mosquitto *mosq, cJSON* data, char *topic)
{
    loginfo("111\n");
    return 0;
}

int func2(struct mosquitto *mosq, cJSON* data, char *topic)
{
    loginfo("222\n");
    return 0;
}

int func3(struct mosquitto *mosq, cJSON* data, char *topic)
{
    loginfo("222\n");
    return 0;
}

int func4(struct mosquitto *mosq, cJSON* data, char *topic)
{
    loginfo("222\n");
    return 0;
}

int test(){
    loginfo("xxxr\n");
	apte_handler_register("func1",func1);
    apte_handler_register("func2",func2);

    apte_handler_register("func3",func3);
    apte_handler_register("func4",func4);
}

void main(){
    printf("Hello, ap\n");
    apte_list_init();
    // test();
    load_modules();
    mqtt_init();
}
