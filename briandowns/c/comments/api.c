// Copyright 2018 Brian J. Downs. All rights reserved.
// Use of this source code is governed by an BSD-style
// license that can be found in the LICENSE file.

#include <ctype.h>
#include <strings.h>
#include <stdint.h>
#include <time.h>

#include <ulfius.h>
#include <jansson.h>

#include "api.h"
#include "http.h"
#include "log.h"
#include "store.h"

#define U_DISABLE_WEBSOCKET

#define COMMENTS_PATH       "/comments"
#define COMMENTS_BY_ID_PATH "/comments/:id"

#define JSON_FIELD_ID    "id"
#define JSON_FIELD_NAME  "name"
#define JSON_FIELD_EMAIL "email"
#define JSON_FIELD_BODY  "body"

/**
 * time_spent takes the start time of a route handler
 * and calculates how long it ran for. It then returns 
 * that value to be logged.
 */
static int time_spent(clock_t start) {                     
    clock_t diff = clock() - start;              
    int msec = diff * 1000 / CLOCKS_PER_SEC; 
    return msec % 1000;
}

/**
 * callback_get_all_comments retrieves all comments from the database.
 */
static int callback_get_all_comments(const struct _u_request *request, struct _u_response *response, void *user_data) {
    clock_t start = clock();

    entry_t *entry = store_get_by_id(store, 1);
    if (entry == NULL) {
        char lm[100];
        sprintf(lm, "no entries found for completed %s request in %dms", COMMENTS_PATH, time_spent(start));
        log_json("info", lm);
        return 1;
    }

    // json_t *json_array = json_array();
    // json_array_append(json_array);
    json_t *json_body;
    json_object_set_new(json_body, "id", json_integer(entry->id)); 
    json_object_set_new(json_body, "name", json_string(entry->name));
    json_object_set_new(json_body, "email", json_string(entry->email));
    json_object_set_new(json_body, "body", json_string(entry->body));

    //json_object_set_new(json_body, "relationships", json_array_set(NULL, 0, NULL));
    ulfius_set_json_body_response(response, HTTP_STATUS_OK, json_body);
    json_decref(json_body);
    store_free_entry(entry);
    
    char lm[100];
    sprintf(lm, "completed %s request in %dms", COMMENTS_PATH, time_spent(start));
    log_json("info", lm);
    return U_CALLBACK_CONTINUE;
}

/**
 * callback_get_comments_by_id
 */
static int callback_get_comments_by_id(const struct _u_request *request, struct _u_response *response, void *user_data) {
    json_t *json_lookup_request = ulfius_get_json_body_request(request, NULL);

    printf("ID: %s\n", request->map_url);

    entry_t entry = store_get_by_id(store, 1);
    if (entry == NULL) {
        char lm[100];
        sprintf(lm, "completed %s request in %dms", COMMENTS_PATH, time_spent(start));
        log_json("info", lm);
    }

    json_t *json_body = json_object();
    json_object_set_new(json_body, "id", json_integer(entry->id)); 
    json_object_set_new(json_body, "name", json_string(entry->name));
    json_object_set_new(json_body, "email", json_string(entry->email));
    json_object_set_new(json_body, "body", json_string(entry->body));
    ulfius_set_json_body_response(response, 200, json_body);
    json_decref(json_lookup_request);
    json_decref(json_body);
    store_free_entry(entry);
    return U_CALLBACK_CONTINUE;
}

int api_add_routes(struct _u_instance *instance) {
    ulfius_add_endpoint_by_val(instance, HTTP_METHOD_GET, COMMENTS_PATH, NULL, 0, &callback_get_all_comments, NULL);
    ulfius_add_endpoint_by_val(instance, HTTP_METHOD_GET, COMMENTS_BY_ID_PATH, NULL, 0, &callback_get_comments_by_id, NULL);
}
