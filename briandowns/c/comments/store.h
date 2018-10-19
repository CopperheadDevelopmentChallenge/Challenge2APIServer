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
 * store_free_entries frees the memory used by the 
 * given array of entry objects
 */
void store_free_entries(entry_t **entries);

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
 * store_get_all retrieves all entries in the data base
 */
entry_t **store_get_all(const store_t *store);

/**
 * store_get_by_id retrieves the record for the given id. The 
 * returned entry_t pointer will need to be freed by the caller.
 */
entry_t *store_get_by_id(const store_t *store, const int id);

/**
 * store_get_by_name
 */
char **store_get_by_name(const store_t *store, const char *name);

/**
 * store_get_by_email
 */
char **store_get_by_email(const store_t *store, const char *name);

/**
 * store_update_entry receives a new entry and updates the 
 * existing entry with the new one
 */
void store_update_entry(store_t *store, const int id);

#endif /* _STORE_H */
