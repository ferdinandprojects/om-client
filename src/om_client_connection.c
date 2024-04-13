#include "om_client_connection.h"

#include "om_client_constants.h"

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

void om_fatal() {
    char str[11 + 3];
    strcpy(str, "om-client: ");
    strcat(str, "%s\n");
    printf(str, strerror(errno));
    fflush(stdout);
}

int om_client_connection_open(const char *addr, uint16_t port, uint16_t timeout) {

    struct sockaddr_in server;

    bzero(&server, sizeof(server));
    server.sin_family = PF_INET;
    server.sin_addr.s_addr = inet_addr(addr);
    server.sin_port = htons(port);

    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if(connect(fd, (struct sockaddr *) &server, sizeof(server)) == -1) {
      om_fatal();
      return -1;
    } 

    struct timeval tv;
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    return fd;
}

int om_client_connection_write(int fd, const char *request, const int request_len) {
    return send(fd, request, request_len, 0);
}
    
int om_client_connection_read(int fd, char *response, int response__len) {
    return recv(fd, response, response__len, 0);
}

void om_client_connection_close(int fd) {
    close(fd);
}

