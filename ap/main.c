#include <stdio.h>
#include <stdlib.h>
#include "mqtt.h"
#include "module.h"
#include "util/util.h"

void main(){
    apte_list_init();
    load_modules();
    show_apte_handler_list();
    mqtt_init();
}
