#include <stdio.h>
#include <stdlib.h>
#include "util.h"

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
