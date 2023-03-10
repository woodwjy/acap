#include <stdio.h>
#include <stdlib.h>
#include "apte_list.h"

apte_handler *apte_handler_list = NULL;


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


apte_handler *apte_handler_register(char *name, int(*func)())
{
    apte_handler *cur = apte_handler_list;
    apte_handler *pre = NULL;

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