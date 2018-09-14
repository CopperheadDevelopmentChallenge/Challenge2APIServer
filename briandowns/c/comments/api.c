#include <ctype.h>
#include <strings.h>

#include <ulfius.h>

#include "api.h"

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

/**
 * callback_get_all_comments
 */
static int callback_get_all_comments(const struct _u_request *request, struct _u_response *response, void *user_data) {
    json_t *json_lookup_request = ulfius_get_json_body_request(request, NULL);
    
    int id = json_string_value(json_object_get(json_lookup_request, "id"));
    const char *name = json_string_value(json_object_get(json_lookup_request, "name"));
    const char *email = json_string_value(json_object_get(json_lookup_request, "email"));
    const char *body = json_string_value(json_object_get(json_lookup_request, "body"));

    json_t *json_body = json_object();
    json_object_set_new(json_body, "id", json_parse_int64(id));
    json_object_set_new(json_body, "name", json_string(source));
    json_object_set_new(json_body, "email", json_string(timestamp));
    json_object_set_new(json_body, "body", json_string(version));
    //json_object_set_new(json_body, "relationships", json_array_set(NULL, 0, NULL));
    ulfius_set_json_body_response(response, 200, json_body);
    json_decref(json_lookup_request);
    json_decref(json_body);
    return U_CALLBACK_CONTINUE;
}

int api_add_routes(struct _u_instance *instance); {
    ulfius_add_endpoint_by_val(instance, HTTP_METHOD_POST, COMMENTS_PATH, NULL, 1, &callback_get_all_comments, NULL);
}
