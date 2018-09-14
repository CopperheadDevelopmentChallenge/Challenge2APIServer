#include <ctype.h>
#include <strings.h>
#include <stdint.h>
#include <time.h>

#include <ulfius.h>
#include <jansson.h>

#include "api.h"
#include "log.h"

#define U_DISABLE_WEBSOCKET

#define COMMENTS_PATH "/comments"
#define COMMENTS_BY_ID_PATH "/comments/:id"

#define HTTP_METHOD_GET    "GET"
#define HTTP_METHOD_POST   "POST"
#define HTTP_METHOD_PUT    "PUT"
#define HTTP_METHOD_DELETE "DELETE"

#define HTTP_STATUS_OK       200
#define HTTP_STATUS_CREATED  201
#define HTTP_STATUS_ACCEPTED 202

int time_spent(clock_t start) {                     
    clock_t diff = clock() - start;              
    int msec = diff * 1000 / CLOCKS_PER_SEC; 
    return msec % 1000;
}

/**
 * callback_get_all_comments retrieves all comments from the database.
 */
static int callback_get_all_comments(const struct _u_request *request, struct _u_response *response, void *user_data) {
    clock_t start = clock();

    char *id = "id";
    char *name = "name";
    char *email = "email"; 
    char *body = "body";

    json_t *json_body = json_object();
    json_object_set_new(json_body, "id", json_integer(id)); 
    json_object_set_new(json_body, "name", json_string(name));
    json_object_set_new(json_body, "email", json_string(email));
    json_object_set_new(json_body, "body", json_string(body));
    //json_object_set_new(json_body, "relationships", json_array_set(NULL, 0, NULL));
    ulfius_set_json_body_response(response, HTTP_STATUS_OK, json_body);
    json_decref(json_body);
    
    char lm[100];
    sprintf(lm, "completed %s request in %dms", COMMENTS_PATH, time_spent(start));
    log_json("info", lm);
    return U_CALLBACK_CONTINUE;
}

/**
 * callback_get_all_comments_with_params
 */
// static int callback_get_all_comments_with_params(const struct _u_request *request, struct _u_response *response, void *user_data) {
//     json_t *json_lookup_request = ulfius_get_json_body_request(request, NULL);

//     char *id = "";
//     const char *name = json_string_value(json_object_get(json_lookup_request, "name"));
//     const char *email = json_string_value(json_object_get(json_lookup_request, "email"));
//     const char *body = json_string_value(json_object_get(json_lookup_request, "body"));

//     json_t *json_body = json_object();
//     json_object_set_new(json_body, "id", json_integer(id)); 
//     json_object_set_new(json_body, "name", json_string(name));
//     json_object_set_new(json_body, "email", json_string(email));
//     json_object_set_new(json_body, "body", json_string(body));
//     //json_object_set_new(json_body, "relationships", json_array_set(NULL, 0, NULL));
//     ulfius_set_json_body_response(response, 200, json_body);
//     json_decref(json_lookup_request);
//     json_decref(json_body);
//     return U_CALLBACK_CONTINUE;
// }

int api_add_routes(struct _u_instance *instance) {
    ulfius_add_endpoint_by_val(instance, HTTP_METHOD_GET, COMMENTS_PATH, NULL, 1, &callback_get_all_comments, NULL);
}
