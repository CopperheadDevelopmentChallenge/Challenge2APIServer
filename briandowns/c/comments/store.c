#include <errno.h>
#include <json-c/json.h>
#include <stdlib.h>
#include <stdio.h>

#include "log.h"
#include "store.h"

void *store_new(const char *filename) {
    FILE* file = NULL;
    long file_size;
    const int block_size = 1;
    size_t read_size;
    char *file_buffer; 

    errno = 0;
    file = fopen(filename, "r");
    if (file == NULL) {
        log_json("error", "store: failed to open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    store = malloc(sizeof(store_t));
    store->data = (char*)malloc(sizeof(char) * file_size); 
    if (store->data == NULL) {
        log_json("error", "store: failed to allocate memory");
        return;
    }

    read_size = fread(store->data, block_size, file_size, file);
    if (read_size != file_size) {
        printf("erno: %d\n", errno);
        log_json("error", "store: failed to read file to end");
        return;
    }
}

void store_free(store_t *store) {
    free(store);
}

char **store_get_by_id(store_t *store) {
    char *buffer = malloc(sizeof(store->size));
    json_object *root_obj = json_tokener_parse(buffer);
    printf("data: %s\n", json_object_to_json_string(root_obj));
    free(buffer);
    return NULL;
}

char **store_get_by_name(store_t *store) {
    return NULL;
}

char **store_get_by_email(store_t *store) {
    return NULL;
}