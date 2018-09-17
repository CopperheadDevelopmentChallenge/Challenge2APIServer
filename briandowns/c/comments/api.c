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
 * get_parameter retrieves a URL parameter from the 
 * given map.
 */
char * print_map(const struct _u_map * map) {
  char * line, * to_return = NULL;
  const char **keys, * value;
  int len, i;
  if (map != NULL) {
    keys = u_map_enum_keys(map);
    for (i=0; keys[i] != NULL; i++) {
      value = u_map_get(map, keys[i]);
      len = snprintf(NULL, 0, "key is %s, value is %s", keys[i], value);
      line = o_malloc((len+1)*sizeof(char));
      snprintf(line, (len+1), "key is %s, value is %s", keys[i], value);
      if (to_return != NULL) {
        len = strlen(to_return) + strlen(line) + 1;
        to_return = o_realloc(to_return, (len+1)*sizeof(char));
        if (strlen(to_return) > 0) {
          strcat(to_return, "\n");
        }
      } else {
        to_return = o_malloc((strlen(line) + 1)*sizeof(char));
        to_return[0] = 0;
      }
      strcat(to_return, line);
      o_free(line);
    }
    return to_return;
  } else {
    return NULL;
  }
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
 * callback_get_comments_by_id retrieves a record from the data
 * store and returns it.
 */
static int callback_get_comments_by_id(const struct _u_request *request, struct _u_response *response, void *user_data) {
    clock_t start = clock();

    const char *record_id = u_map_get(request->map_url, "record_id");
    printf("here %s\n", record_id);
    if (record_id == NULL) 
        printf("completely null...");

    int i_param = atoi(record_id);
    entry_t *entry = store_get_by_id(store, i_param);
    if (entry == NULL) {
        char lm[100];
        sprintf(lm, "completed %s request in %dms", COMMENTS_PATH, time_spent(start));
        log_json("info", lm);
    }

    json_t *json_body = json_object();
    json_object_set_new(json_body, JSON_FIELD_ID, json_integer(entry->id)); 
    json_object_set_new(json_body, JSON_FIELD_NAME, json_string(entry->name));
    json_object_set_new(json_body, JSON_FIELD_EMAIL, json_string(entry->email));
    json_object_set_new(json_body, JSON_FIELD_BODY, json_string(entry->body));
    ulfius_set_json_body_response(response, 200, json_body);
    json_decref(json_body);
    store_free_entry(entry);

    char lm[100];
    sprintf(lm, "completed %s request in %dms", COMMENTS_BY_ID_PATH, time_spent(start));
    log_json("info", lm);

    return U_CALLBACK_CONTINUE;
}

/**
 * callback_default is used to handled calls that don't have
 * a matching route. Returns an expected 404.
 */
int callback_default(const struct _u_request *request, struct _u_response *response, void *user_data) {
    ulfius_set_string_body_response(response, 404, "page not found");
    return U_CALLBACK_CONTINUE;
}

int api_add_routes(struct _u_instance *instance) {
    ulfius_add_endpoint_by_val(instance, HTTP_METHOD_GET, "", COMMENTS_PATH, 0, &callback_get_all_comments, NULL);
    ulfius_add_endpoint_by_val(instance, HTTP_METHOD_GET, "", COMMENTS_BY_ID_PATH, 0, &callback_get_comments_by_id, NULL);

    ulfius_set_default_endpoint(instance, &callback_default, NULL);
}
