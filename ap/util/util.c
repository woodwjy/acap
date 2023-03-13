#include <stdio.h>
#include <stdlib.h>
#include "util.h"

apte_handler *apte_handler_list = NULL;

typedef unsigned short 		char_t;

char_t *getJsonVar(cJSON *object, char_t *var, char_t *defaultGetValue)
{
	cJSON	*sp;
 
	if ((sp = cJSON_GetObjectItem(object, var)) != NULL) {
		if (sp->valuestring) {
			return sp->valuestring;
		} else {
			return "";
		}
	}
	return defaultGetValue;
}



char * safe_strdup(const char *s) {
	char * ret = NULL;
	if (!s) {
		exit(1);
	}
	ret = strdup(s);
	if (!ret) {
		exit(1);
	}
	return ret;
}


void apte_list_init(void){
    apte_handler_list = NULL;
}

void show_apte_handler_list()
{
    apte_handler *cur = apte_handler_list;

    printf("handler list :\n");
    while(cur != NULL){
        printf("[name]%s\n", cur->name);
        cur = cur->next;
    }

}

apte_handler *apte_handler_register(char *name, int(*func)())
{
    apte_handler *cur, *pre;
    pre = NULL;
    cur = apte_handler_list;
    // apte_handler *cur = apte_handler_list;
    // apte_handler *pre = NULL;

    while (cur != NULL) {
        pre = cur;
        cur = cur->next;
    }
    
    cur = malloc(sizeof(apte_handler));
    memset(cur, 0, sizeof(apte_handler));
    cur->name = safe_strdup(name);
    cur->handler = func;

    if (pre == NULL) 
    {
        apte_handler_list = cur;
    } 
    else 
    {
        pre->next = cur;
    }

    return cur;
}