// Copyright 2018 Brian J. Downs. All rights reserved.
// Use of this source code is governed by an BSD-style
// license that can be found in the LICENSE file.

#include <ctype.h>
#include <string.h>
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
#define COMMENTS_BY_ID_PATH "/comments/:record_id"

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

    entry_t **entries = store_get_all(store);
    if (entries == NULL) {
        char lm[100];
        sprintf(lm, "failed to retrieve all entries in %s request in %dms", COMMENTS_PATH, time_spent(start));
        log_json("error", lm);
        return 1;
    } 

    json_t *array = json_array();
    for (int i = 0; i < store->size; i++) {
        json_t *o = json_pack("{s:i, s:s, s:s, s:s}", "id", 
            entries[i]->id, "name", 
            entries[i]->name, "email", 
            entries[i]->email, "body", 
            entries[i]->body);
        json_array_append_new(array, o);
    }
    json_t *json_body = json_pack("{s:o}", "data", array);
    ulfius_set_json_body_response(response, HTTP_STATUS_OK, json_body);
    json_decref(json_body);
    json_decref(array);
    store_free_entries(entries);
    
    char lm[100];
    sprintf(lm, "completed GET %s request in %dms", COMMENTS_PATH, time_spent(start));
    log_json("info", lm);
    return U_CALLBACK_CONTINUE;
}

/**
 * callback_get_comments_by_id retrieves a record from the data
 * store and returns it.
 */
static int callback_get_comments_by_id(const struct _u_request *request, struct _u_response *response, void *user_data) {
    clock_t start = clock();

    const char *record_id = u_map_get(request->map_url, "record_id");
    if (record_id == NULL) {
        char lm[100];
        sprintf(lm, "null input for %s. request in %dms", COMMENTS_PATH, time_spent(start));
        log_json("error", lm);
        return 1;
    }

    int i_param = atoi(record_id);
    json_t *json_body = json_object();

    entry_t *entry = store_get_by_id(store, i_param);
    if (entry == NULL) {
        json_object_set_new(json_body, NULL, NULL);
        ulfius_set_json_body_response(response, HTTP_STATUS_OK, json_body);
        json_decref(json_body);
        log_json("info", "record not found");
        char lm[150];
        sprintf(lm, "completed GET %s request in %dms", COMMENTS_BY_ID_PATH, time_spent(start));
        log_json("info", lm);
        return 1;
    }
    
    json_object_set_new(json_body, JSON_FIELD_ID, json_integer(entry->id)); 
    json_object_set_new(json_body, JSON_FIELD_NAME, json_string(entry->name));
    json_object_set_new(json_body, JSON_FIELD_EMAIL, json_string(entry->email));
    json_object_set_new(json_body, JSON_FIELD_BODY, json_string(entry->body));
    ulfius_set_json_body_response(response, HTTP_STATUS_OK, json_body);
    json_decref(json_body);
    store_free_entry(entry);

    char lm[100];
    sprintf(lm, "completed GET %s request in %dms", COMMENTS_BY_ID_PATH, time_spent(start));
    log_json("info", lm);

    return U_CALLBACK_CONTINUE;
}

/**
 * callback_delete_comments_by_id retrieves a record from the data
 * store and returns it.
 */
static int callback_delete_comments_by_id(const struct _u_request *request, struct _u_response *response, void *user_data) {
    clock_t start = clock();

    const char *record_id = u_map_get(request->map_url, "record_id");
    if (record_id == NULL) {
        char lm[100];
        sprintf(lm, "null input for %s. DELETE request in %dms", COMMENTS_BY_ID_PATH, time_spent(start));
        log_json("error", lm);
        return 1;
    }

    int i_param = atoi(record_id);

    store_update_entry(store, i_param);

    ulfius_set_empty_body_response(response, HTTP_STATUS_ACCEPTED);

    char lm[100];
    sprintf(lm, "completed DELETE %s request in %dms", COMMENTS_BY_ID_PATH, time_spent(start));
    log_json("info", lm);

    return U_CALLBACK_CONTINUE;
}

/**
 * callback_default is used to handled calls that don't have
 * a matching route. Returns an expected 404.
 */
int callback_default(const struct _u_request *request, struct _u_response *response, void *user_data) {
    ulfius_set_string_body_response(response, HTTP_STATUS_NOT_FOUND, "page not found");
    return U_CALLBACK_CONTINUE;
}

int api_add_routes(struct _u_instance *instance) {
    ulfius_add_endpoint_by_val(instance, HTTP_METHOD_GET, "", COMMENTS_PATH, 0, &callback_get_all_comments, NULL);
    ulfius_add_endpoint_by_val(instance, HTTP_METHOD_GET, "", COMMENTS_BY_ID_PATH, 0, &callback_get_comments_by_id, NULL);
    ulfius_add_endpoint_by_val(instance, HTTP_METHOD_DELETE, "", COMMENTS_BY_ID_PATH, 0, &callback_delete_comments_by_id, NULL);

    ulfius_set_default_endpoint(instance, &callback_default, NULL);
}
