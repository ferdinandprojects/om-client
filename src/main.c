#include "om_client.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {

    char* command = "CHECK ";
    char* uuid = "aa7a29e9-f57f-45fa-b8da-732920d1868e";
    char* solution = "31162522";

    char request[100];
    request[0] = '\0';

    strcat(request, command);
    strcat(request, uuid);
    strcat(request, " ");
    strcat(request, solution);

    int len = strlen(request);

    char response[1000];
    int response_len;

    om_client_invoke("127.0.0.1", 5005, request, len, response, &response_len);

    response[response_len] = '\0';
    printf("response length: %d\n", response_len);
    printf("BODY: %s\n", response);
}

