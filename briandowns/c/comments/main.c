#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include <sqlite3.h>
#include <sqlite3ext.h>
#include <ulfius.h>

#include "api.h"
#include "config.h"
#include "log.h"

#define STR1(x) #x
#define STR(x) STR1(x)

#define DATA_FILE "data.json"

size_t get_file_size(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

/**
 * sig_handler
 */
static void sig_handler(int dummy) {
    control = 1;
    exit(EXIT_FAILURE);
}

int main() {
    signal(SIGINT, sig_handler);

    enum logger_format lf;

    size_t filesize = get_file_size(DATA_FILE);
    int f = open(DATA_FILE, O_RDONLY, 0);
    if (!f) {
        perror("unable to open " DATA_FILE);
        exit EXIT_FAILURE;
    }
    //Execute mmap
    void* mmapped_data = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, f, 0);
    if (mmapped_data == MAP_FAILED) {
        perror("");
        return EXIT_FAILURE;
    }
    //Write the mmapped data to stdout 
    write(STDOUT_FILENO, mmapped_data, filesize);
    //Cleanup
    int rc = munmap(mmapped_data, filesize);
    if (rc != 0) {
        perror("");
        return EXIT_FAILURE; 
    }
    close(fd);

    config_t *config = config_load();

    struct _u_instance instance;

    if (ulfius_init_instance(&instance, config->http_port, NULL, NULL) != U_OK) {
        y_log_message(Y_LOG_LEVEL_ERROR, "error ulfius_init_instance, abort");
        return EXIT_FAILURE;
    } 

    if (api_add_routes(&instance) != 0) {
        perror("unable to add routes");
        return EXIT_FAILURE;
    }

    if (ulfius_start_framework(&instance) == U_OK) {
        printf("Start framework on port %d\n", instance.port);
        getchar();
    } else {
        fprintf(stderr, "error starting framework\n");
    }

    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);
    
    config_free(config);

    return EXIT_SUCCESS;
}
