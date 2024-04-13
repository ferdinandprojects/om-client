#ifndef OM_CLIENT_CONNECTION_H
#define OM_CLIENT_CONNECTION_H

#include <stdint.h>

int om_client_connection_open(const char *addr, uint16_t port, uint16_t timeout);

int om_client_connection_write(int fd, const char *request, const int request_len);
    
int om_client_connection_read(int fd, char *response, int response__len);

void om_client_connection_close(int fd);

#endif

