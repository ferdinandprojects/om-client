#ifndef OM_CLIENT_H
#define OM_CLIENT_H

#include <stdint.h>

#define OM_CLIENT_RESPONSE_OK 0
#define OM_CLIENT_RESPONSE_FAIL -1
#define OM_CLIENT_INTERNAL_ERROR -2

int om_client_invoke(const char *addr, uint16_t port, const char *request_body, const int request_body_len, char *response_body, int *response_body_len);

#endif

