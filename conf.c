#include "conf.h"
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include "log.h"

struct Conf* initConf (int argc, char** argv) {
    zlog_category_t* c = getLogCategory ();

    struct Conf* conf = (struct Conf*) malloc (sizeof(conf));

    char* cValue = NULL;
    int arg;

    while (-1 != (arg = getopt (argc, argv, "N:k:o:l:"))) {
        switch (arg) {
            case 'N':
                cValue = strdup(optarg);

                zlog_debug(c, "Find arg N: %s", cValue);

                char* pair[2];
                char* tok = NULL;
                int index = 0;

                while ( (tok = strsep(&cValue, ":")) && index < 2 ) {
                    pair[index++] = tok;
                }

                zlog_debug(c, "N arg split into: [%s, %s]", pair[0], pair[1]);

                if ( 0 == strncmp ("TCP", pair[0], strlen ("TCP")) ) {
                    conf->server_mode = TCP;
                    conf->server_interface.port = atoi (pair[1]);

                    zlog_debug(c, "Server setting: TCP:%d", conf->server_interface.port);
                } else if ( 0 == strncmp ("UNIX", pair[0], strlen ("UNIX")) ) {
                    conf->server_mode = UNIX;
                    conf->server_interface.path = strdup(pair[1]);

                    zlog_debug(c, "Server setting: UNIX:%s", conf->server_interface.path);
                }

                free(cValue);

                break;
            case 'k':
                cValue = optarg;
                char* key_pattern = (char*) malloc (sizeof (char) * strlen (cValue));

                conf->key_pattern = key_pattern;

                break;
            case 'o':
                cValue = optarg;
                char* output_pattern = (char*) malloc (sizeof (char) * strlen (cValue));

                conf->output_pattern = output_pattern;

                break;
            case 'l':
                cValue = optarg;
                char* log = (char*) malloc (sizeof (char) * strlen (cValue));

                conf->log_conf_path = log;

                break;
            default:
                break;
        }
    }

    return conf;
}
