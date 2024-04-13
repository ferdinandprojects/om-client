#include "om_client.h"

#include "om_client_constants.h"
#include "om_client_connection.h"
#include "om_client_constants.h"

#include "om_client_connection.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static const char *PACKET_START = "POST /om\r\nContent-Length: ";

static const int PACKET_START_LEN = 26;

static const char* START_RESPONSE_OK = "HTTP/1.1 200 OK\r\nContent-Length: ";

static const int START_RESPONSE_OK_LEN = 33;

static const char* OM_RESPONSE_BAD_REQUEST = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";

static const int OM_RESPONSE_BAD_REQUEST_LENGTH = 47;

static const char* OM_RESPONSE_ERROR = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";

static const int OM_RESPONSE_ERROR_LENGTH = 57; 


static void om_client_send(int fd, const char* data, const int len) {

    char len_str[5]; //5 - max len of content-length string
    sprintf(len_str, "%d", len);
    const int len_str_size = strlen(len_str);

    char request[PACKET_START_LEN + len_str_size + 4 + len];

    memcpy(request, PACKET_START, PACKET_START_LEN);
    memcpy(request + PACKET_START_LEN, len_str, len_str_size);
    memcpy(request + PACKET_START_LEN + len_str_size, "\r\n\r\n", 4);
    memcpy(request + PACKET_START_LEN + len_str_size + 4, data, len);

    om_client_connection_write(fd, request, PACKET_START_LEN + len_str_size + 4 + len);
}

static int om_client_receive(int fd, char *data, int *len) {

    char response[OM_CLIENT_MAX_RESPONSE_LENGTH];
    int p = 0;
    int body_start = -1;
    int content_len = -1;

    while (body_start == -1 || p - body_start < content_len) {
   
        if (p >= OM_CLIENT_MAX_RESPONSE_LENGTH)
            return OM_CLIENT_INTERNAL_ERROR;

        p += om_client_connection_read(fd, response + p, OM_CLIENT_MAX_RESPONSE_LENGTH - p);

        if (body_start == -1 && p > START_RESPONSE_OK_LEN) { 

            if(memcmp(response, START_RESPONSE_OK, START_RESPONSE_OK_LEN) != 0)
                return OM_CLIENT_RESPONSE_FAIL;

            char* c = strstr(response + START_RESPONSE_OK_LEN, "\r\n\r\n");
            
            if(c != NULL) {
                body_start = c - response + 4;
                content_len = atoi(response + START_RESPONSE_OK_LEN);
            }
        }
    }

    memcpy(data, response + body_start, content_len);
    *len = content_len;
    om_client_connection_close(fd);

    return OM_CLIENT_RESPONSE_OK;
}

int om_client_invoke(const char *addr, uint16_t port, const char *request_body, const int request_body_len, char *response_body, int *response_body_len) {

    char response[OM_CLIENT_MAX_RESPONSE_LENGTH];
    int fd = om_client_commection_open(addr, port, 5);
    om_client_send(fd, request_body, request_body_len);
    om_client_receive(fd, response_body, response_body_len);
    om_client_connection_close(fd);
    return 0;
}

