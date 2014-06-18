#include "log.h"

zlog_category_t* c;

int initLog () {
    int rc;

    rc = zlog_init("zlog.conf");
    if (rc) {
        zlog_info(c, "init failed\n");
        return -1;
    }

    c = zlog_get_category("my_cat");
    if (!c) {
        zlog_info(c, "get cat fail\n");
        zlog_fini();
        return -2;
    }

    zlog_info(c, "hello, zlog");

    return 0;
}

void finishLog () {
    zlog_fini();
}

zlog_category_t* getLogCategory () {
    if (!c) {
        initLog ();
    }

    return c;
}
