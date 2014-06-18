#ifndef Jack_0614_LOG_HEADER
#define Jack_0614_LOG_HEADER

#include "zlog/include/zlog.h"

int initLog ();
void finishLog ();
zlog_category_t* getLogCategory ();
#endif

