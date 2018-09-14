#ifndef _CONFIG_H
#define _CONFIG_H

/**
 * config_t contains the application configuration
 */
typedef struct {
    char *http_port;
} config_t;

/** 
 * config_load loads all relevant environment variables
 * and puts them into a config_t pointer. The caller needs
 * to free this memory.
 */
config_t *config_load();

/**
 * config_free frees the memory used by the config_t pointer
 * that gets passed in.
 */
void config_free(config_t *conf);

#endif
