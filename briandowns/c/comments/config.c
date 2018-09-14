#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "config.h"

#define HTTP_PORT "HTTP_PORT"

config_t *config_load() {
    config_t *config = malloc(sizeof(config_t));
    
    char *http_port = getenv(HTTP_PORT);
    if (strlen(http_port) == 0) {
        perror("HTTP_PORT is empty");
        return NULL;
    }
    if (!(config->http_port = malloc(strlen(http_port) + 1))) {
        perror("allocation for http_port failed");
        return NULL;
    }
    strcpy(config->http_port, http_port);

    return config;
}

void config_free(config_t *conf) {
    free(conf->http_port);
    free(conf);
}