#include <syslog.h>

#define DEBUG

#ifdef DEBUG
#define log(fmt, args...)  \
printf("[%s](%d)" fmt , __func__, __LINE__, ## args)
#define logerr log
#define logwarn log
#define loginfo log
#else
#define logerr(fmt, args...)  \
syslog(LOG_ERR|LOG_USER,"[%s](%d)" fmt , __func__, __LINE__, ## args)

#define logwarn(fmt, args...)  \
syslog(LOG_WARNING,"[%s](%d)" fmt , __func__, __LINE__, ## args)


#define loginfo(fmt, args...)  \
syslog(LOG_INFO,"[%s](%d)" fmt , __func__, __LINE__, ## args)
#endif


void UserServMain();
void DiscServMain();
void ApManagerMain();
void ScriptManagerMain();

