// Copyright 2018 Brian J. Downs. All rights reserved.
// Use of this source code is governed by an BSD-style
// license that can be found in the LICENSE file.

#include <json-c/json.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <time.h>

#include "log.h"

log_t *log_new(enum logger_format lf) {
    log_t *lt = malloc(sizeof(log_t));
    lt->log_format = lf;
    return lt;
}

void log_json(const char* tag, char* message) {
    struct json_object *json_log;
    json_log = json_object_new_object();

    char t_buff[24];
    time_t now = time(NULL);
    strftime(t_buff, 24, "%Y-%m-%d %H:%M:%S ", localtime(&now));
    json_object_object_add(json_log, "timestamp", json_object_new_string(t_buff));
    json_object_object_add(json_log, tag, json_object_new_string(message));
    printf("%s\n", json_object_to_json_string(json_log));
}

void log_standard(const char* tag, char* message) {
    char log_line[4096];

    time_t now = time(NULL);
    strftime(log_line, 24, "%Y-%m-%d %H:%M:%S ", localtime(&now));

    for (char *p = strtok(message, "\r\n"); p != NULL; p = strtok(NULL, "\r\n")) {
        if (strstr(p, "HTTP/1") != NULL) {
            strcat(log_line, p);
            strcat(log_line, " ");
        }

        if (strstr(p, "Host") != NULL) {
            strcat(log_line, p);
            strcat(log_line, " ");
        }

        if (strstr(p, "User") != NULL) {
            strcat(log_line, p);
            strcat(log_line, " ");
        }
    }

    printf("%s\n", log_line);
}

void log_to_stdout(int lf, const char* tag, char* message) {
    switch(lf) {
    case 0:
        log_standard(tag, message);
    case 1:
        log_json(tag, message);
    }
}
