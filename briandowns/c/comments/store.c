#include <errno.h>
#include <json-c/json.h>
#include <stdlib.h>
#include <stdio.h>

#include "log.h"
#include "store.h"

#define PRIMARY_KEY "data"

entry_t *store_new_entry() {
    entry_t *entry = malloc(sizeof(entry_t));

    return entry;
}

void *store_new(const char *filename) {
    store_t *store = malloc(sizeof(store_t));
    struct json_object *data = json_object_from_file(filename);
    store->data = json_object_object_get(data, PRIMARY_KEY);
    store->size = json_object_array_length(store->data);
    return store;
}

void store_free(store_t *store) {
    free(store->data);
    free(store); 
}

entry_t *store_get_by_id(const store_t *store, int id) {
    struct json_object *data_obj, *json_obj_id, *json_obj_name, *json_obj_email, *json_obj_body;
    for (int i = 0; i < store->size; i++) {
        // get the i-th object in medi_array
        data_obj = json_object_array_get_idx(store->data, i);
        // get the name attribute in the i-th object
        json_obj_id = json_object_object_get(data_obj, "id");
        if (json_object_get_int(json_obj_id) == id) {
            json_obj_name = json_object_object_get(data_obj, "name");
            json_obj_email = json_object_object_get(data_obj, "email");
            json_obj_body = json_object_object_get(data_obj, "body");

            entry_t *entry = store_new_entry();
            entry->id = json_object_get_int(json_obj_id);
            entry->name = json_object_get_string(json_obj_name);
            entry->email = json_object_get_string(json_obj_email);
            entry->body = json_object_get_string(json_obj_body);

            return entry;
        }
    }
    return NULL;
}

char **store_get_by_name(const store_t *store, const char *name) {
    return NULL;
}

char **store_get_by_email(const store_t *store, const char *name) {
    return NULL;
}