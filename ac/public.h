#include <syslog.h>
#include "apManager.h"
#include "discServ.h"
#include "mqtt_pub.h"
#include "mqtt_sub.h"
#include "scriptManager.h"
#include "userServ.h"
#include "sql.h"

int GetRunningState();

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


void UserServMain();
void DiscServMain();
void ApManagerMain();
void ScriptManagerMain();

