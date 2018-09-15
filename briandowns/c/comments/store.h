#ifndef _STORE_H
#define _STORE_H

#include <json-c/json.h>
#include <stdlib.h>

/**
 * store_t
 */
typedef struct {
    struct json_object *data;
    size_t size;
} store_t;

store_t *store;

/**
 * store_new
 */
void *store_new();

/**
 * store_free frees the memory used by the store object
 */
void store_free(store_t *store);

/**
 * store_get_by_id
 */
json_object *store_get_by_id(const store_t *store, const int id);

/**
 * store_get_by_name
 */
char **store_get_by_name(const store_t *store, const char *name);

/**
 * store_get_by_email
 */
char **store_get_by_email(const store_t *store, const char *name);

#endif
