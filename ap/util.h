#include "../ext/cJSON.h"
typedef unsigned short 		char_t;

#define DEBUG

#ifdef DEBUG
#define logw(fmt, args...)  \
printf("[%s](%d)" fmt , __func__, __LINE__, ## args)
#define logerr logw
#define logwarn logw
#define loginfo logw
#else
#define logerr(fmt, args...)  \
syslog(LOG_ERR|LOG_USER,"[%s](%d)" fmt , __func__, __LINE__, ## args)

#define logwarn(fmt, args...)  \
syslog(LOG_WARNING,"[%s](%d)" fmt , __func__, __LINE__, ## args)


#define loginfo(fmt, args...)  \
syslog(LOG_INFO,"[%s](%d)" fmt , __func__, __LINE__, ## args)
#endif


char_t *getJsonVar(cJSON *object, char_t *var, char_t *defaultGetValue);