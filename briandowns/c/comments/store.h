#ifndef _STORE_H
#define _STORE_H

#include <stdlib.h>

/**
 * store_t
 */
typedef struct {
    char *data;
    size_t size;
} store_t;

/**
 * store_new
 */
store_t *store_new();

/**
 * store_free frees the memory used by the store object
 */
void store_free(store_t *store);

/**
 * store_get_by_id
 */
char **store_get_by_id();

/**
 * store_get_by_id
 */
char **store_get_by_name();

/**
 * store_get_by_id
 */
char **store_get_by_email();

#endif
