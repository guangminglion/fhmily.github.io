// system headers
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>

#include "conf.h"
// third party headers
#include "log.h"

#define NAME "/tmp/socket"

#define HTTP_HEADER_200 "HTTP/1.1 200 OK\r\n\
Server: YID/0.1\r\n\
Content-Length: %d\r\n\
\r\n"

#define END "\r\n\r\n"

struct timeval cache;
int counter;

struct timeval GetTimeStamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv;
}

int tvEqual (struct timeval* a, struct timeval* b) {
    if (a->tv_sec == b->tv_sec) { // && a->tv_sec == b->tv_sec) {
        return 1;
    }

    return 0;
}

int getID () {
    struct timeval currentTV = GetTimeStamp();

    if (0 == tvEqual(&currentTV, &cache)) {
        cache = currentTV;
        counter = 0;
    }

    return counter++;
}



int main(int argc, char *argv[])
{
    zlog_category_t* c = getLogCategory ();

    struct Conf* conf = initConf(argc, argv);
    
    int end_size = strlen(END);

    int sock, msgsock, rval;
    struct sockaddr_in server;
    char buf[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        zlog_error(c, "opening stream socket");
        exit(1);
    }

    server = (struct sockaddr_in) { .sin_family = AF_INET, .sin_port = htons(conf->server_interface.port), .sin_addr.s_addr = INADDR_ANY };

    if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr))) {
        zlog_error(c, "binding socket fail");
        exit(1);
    }

    zlog_info(c, "Binded port: %d", conf->server_interface.port);

    char http_header_buffer[1024];
    char http_body_buffer[1024];

    listen(sock, 100);
    for (;;) {
        zlog_info(c, "Waiting for new connection");
        msgsock = accept(sock, 0, 0);
        if (msgsock == -1)
            zlog_info(c, "error while accepting new connection");
        else do {
            zlog_info(c, "accepted a new connection");
            bzero(buf, sizeof(buf));
            if ((rval = recv(msgsock, buf, 1024, 0)) < 0)
                zlog_info(c, "reading stream message");
            else if (rval == 0)
                zlog_info(c, "Ending connection\n");
            else {
                zlog_info(c, "-->%s\n", buf);

                if (0 == strncmp(END, &buf[rval - end_size], end_size)) {

                    int id = getID();
                    sprintf(http_body_buffer, "%d", id);

                    sprintf(http_header_buffer, HTTP_HEADER_200, (int) (strlen(http_body_buffer) * sizeof(char)));

                    send(msgsock, http_header_buffer, strlen(http_header_buffer), 0);
                    send(msgsock, http_body_buffer, strlen(http_body_buffer), 0);

                    zlog_info(c, "Current id: %d", id);

                    break;
                }

            }
        } while (rval > 0);

        close(msgsock);

        zlog_info(c, "socket connection closed");
    }
    close(sock);

    finishLog();
}

