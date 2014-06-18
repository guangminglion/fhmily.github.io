// system headers
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>

// third party headers
#include "log.h"

#define NAME "/tmp/socket"
zlog_category_t* c;

#define HTTP200 "HTTP/1.1 200 OK\r\n\
Server: YID/0.1\r\n\
\r\n\
OK"

int main(int argc, char *argv[])
{
    zlog_category_t* c = getLogCategory ();

    unlink(NAME);

    int sock, msgsock, rval;
    struct sockaddr_un server;
    char buf[1024];

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        zlog_error(c, "opening stream socket");
        exit(1);
    }
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, NAME);
    if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) {
        zlog_error(c, "binding stream socket");
        exit(1);
    }
    zlog_info(c, "Socket has name %s\n", server.sun_path);

    listen(sock, 100);
    for (;;) {
        msgsock = accept(sock, 0, 0);
        if (msgsock == -1)
            zlog_error(c, "accept");
        else do {
            bzero(buf, sizeof(buf));
            if ((rval = read(msgsock, buf, 1024)) < 0)
                zlog_error(c, "reading stream message");
            else if (rval == 0)
                zlog_info(c, "Ending connection\n");
            else {
                zlog_info(c, "-->%s\n", buf);
                write(msgsock, HTTP200, sizeof(HTTP200));
            }
        } while (rval > 0);
        close(msgsock);
    }
    close(sock);
    unlink(NAME);

    finishLog();
}

