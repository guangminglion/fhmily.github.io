#ifndef JACK_0614_CONF_HEADER
#define JACK_0614_CONF_HEADER

struct Conf {
    // server configurations
    enum Network {TCP, UNIX} server_mode; // server interface working mode
    union {
        int port; // for TCP
        char* path; // for UNIX domain socket
    } server_interface;

    // API output configurations
    char* key_pattern;
    char* output_pattern;

    // log configuration
    char* log_conf_path;
} Conf, *ConfPtr;

// initial configuration struct
struct Conf* initConf (int, char**);

#endif
