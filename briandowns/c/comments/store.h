// Copyright 2018 Brian J. Downs. All rights reserved.
// Use of this source code is governed by an BSD-style
// license that can be found in the LICENSE file.

#ifndef _STORE_H
#define _STORE_H

#include <stdlib.h>

/**
 * entry_t
 */
typedef struct {
    int id;
    const char *name;
    const char *email;
    const char *body;
} entry_t;

/**
 * store_new_entry
 */
entry_t *store_new_entry();

/**
 * store_free_entry frees the memory used by the 
 * entry object.
 */
void store_free_entry(entry_t *entry);

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
entry_t *store_get_by_id(const store_t *store, int id);

/**
 * store_get_by_name
 */
char **store_get_by_name(const store_t *store, const char *name);

/**
 * store_get_by_email
 */
char **store_get_by_email(const store_t *store, const char *name);

#endif /* _STORE_H */
